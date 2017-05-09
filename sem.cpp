extern "C"
{
#include "symbol.h"
}
#include "sem.h"
#include "ast.h"

#include <stdio.h>
void printSymbolTable ()
{
    Scope       * scp;
    SymbolEntry * e;
    SymbolEntry * args;

    scp = currentScope;
    if (scp == NULL)
        printf("no scope\n");
    else
        while (scp != NULL) {
            printf("scope: ");
            e = scp->entries;
            while (e != NULL) {
                if (e->entryType == ENTRY_TEMPORARY)
                    printf("$%d", e->u.eTemporary.number);
                else
                    printf("%s", e->id);
                switch (e->entryType) {
                    case ENTRY_FUNCTION:
                        printf("(");
                        args = e->u.eFunction.firstArgument;
                        while (args != NULL) {
                            printMode(args->u.eParameter.mode);
                            printf("%s : ", args->id);
                            printType(args->u.eParameter.type);
                            args = args->u.eParameter.next;
                            if (args != NULL)
                                printf("; ");
                        }
                        printf(") : ");
                        printType(e->u.eFunction.resultType);
                        break;
#ifdef SHOW_OFFSETS
                    case ENTRY_VARIABLE:
                        printf("[%d]", e->u.eVariable.offset);
                        break;
                    case ENTRY_PARAMETER:
                        printf("[%d]", e->u.eParameter.offset);
                        break;
                    case ENTRY_TEMPORARY:
                        printf("[%d]", e->u.eTemporary.offset);
                        break;
#endif
                }
                e = e->nextInScope;
                if (e != NULL)
                    printf(", ");
            }
            scp = scp->parent;
            printf("\n");
        }
    printf("----------------------------------------\n");
}

int sem_check_fdef(ASTfdef* func){
        if(func==NULL){
                cout << "WAIT WHAT";
                exit(-1);
        }
        openScope();

        ASTheader* header = func->header;
        SymbolEntry* f = newFunction(header->identifier.c_str());
        ASTparam *iter = header->paramlist;
        while(iter != NULL){
                for(auto st : *iter->identifiers){
                        if(iter->byref)
                                newParameter(st.c_str(),iter->p,PASS_BY_REFERENCE,f);
                        else
                                newParameter(st.c_str(),iter->p,PASS_BY_VALUE,f);
                }
                iter = iter->next;
        }

        endFunctionHeader(f,header->type);
        printSymbolTable();
        sem_check_stmt(func->body);
        closeScope();
        return 1;
}
int sem_check_stmt(ASTstmt* stmt){
        if(stmt==NULL){
                //cout << "DONE" << endl;
                exit(0);
        }
        //TODO: maybe eliminate recursion , for auto i : stmts in func do sem_check_stmt(i);
        //cout << "NEXT ONE" << endl;
        switch(stmt->type){
                case TSKIP: break;
                case TPC: break;
                case TIF: break;
                case TFDEF:
                        sem_check_fdef(stmt->def);
                        break;
                case TFDECL:
                        //TODO: think about function declarations.
                        break;
                case TEXIT: break;
                case TRET: break;
                case TFCALL: break;
                case TCONTM: break;
                case TCONT: break;
                case TBREAKM: break;
                case TBREAK: break;
                case TDECL:
                        //cout << "Hello" << endl;
                        for(auto st : *stmt->identifiers){
                                newVariable(st.c_str(),stmt->t);
                        }
                        break;
                case TASSIGN: break;
                case TLOOP: break;
        }
        //printSymbolTable();
        sem_check_stmt(stmt->tail);
        return 0;
}
