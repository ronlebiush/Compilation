#include "cg.hpp"
#include "source.hpp"

class LlvmCodeHandler{
    CodeBuffer codeBuffer;
    int RegNum = 0;
    

public:
    LlvmCodeHandler(const CodeBuffer& other):codeBuffer(other){};
    std::string freshVar();
    void handle_binop(NumVarNode& res_exp, const NumVarNode& L_exp, const NumVarNode& R_exp, const std::string op);
    void handle_relop(BoolVarNode& res_exp, const NumVarNode& L_exp, const NumVarNode& R_exp, const string op);



};;