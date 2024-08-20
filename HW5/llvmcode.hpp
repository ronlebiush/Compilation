#include "cg.hpp"
#include "source.hpp"

class LlvmCodeHandler{
    

public:
    CodeBuffer codeBuffer;
    int RegNum = 0;
    stack<string> cont_labels;
    stack<string> break_labels;

    LlvmCodeHandler()=default;
    std::string freshVar();
    void handle_binop(Node* res_exp, Node* L_exp, Node* R_exp, std::string op);
    void handle_relop(BoolVarNode* res_exp, Node* L_exp, Node* R_exp, string op);
    string allocate_var(string type, string id, string assigningVar);
    string getLlvmType(string type);

};