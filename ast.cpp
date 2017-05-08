#include "ast.h"
using namespace std;
ASTstmt::ASTstmt(stmt_type type, ASTstmt *body,ASTstmt *tail,string label){
    this->type = type;
    this->body = body;
    this->tail = tail;
    this->label = label;
}

ASTExpr::ASTExpr(char op,ASTlval* operand,int constval,ASTExpr *left,ASTExpr *right){
    //cout << "hello world" << endl;
    this->op = op;
    this->operand=operand;
    this->constant_val=constval;
    this->left = left;
    this->right = right;

}

ASTfcall::ASTfcall(string identifier){
    this->identifier = identifier;
    parameters = new vector<ASTExpr*>;
}
ASTlval::ASTlval(bool constant,string identifier){
    this->constant = constant;
    this->identifier = identifier;
    this->indices = new vector<ASTExpr*>();
}

ASTfdef::ASTfdef(ASTheader *header,ASTstmt *body){
    this->header = header;
    this->body = body;
}

ASTheader::ASTheader(Type type,ASTparam *list){
    if(type == typeVoid){
            cout << "PROCEDURE HEADER FOUND" << endl;
    }
    this->type = type;
    this->paramlist = list;
}

ASTparam::ASTparam(vector<string>* id, Type p, ASTparam *next){
    this->identifiers = id;
    this->p = p;
    this->indices = new vector<int>();
    this->next = next;
}
