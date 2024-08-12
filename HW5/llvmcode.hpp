#include "cg.hpp"
#include "source.hpp"

class LlvmCodeHandler{
    CodeBuffer codeBuffer;
    int RegNum = 0;
    

public:
    LlvmCodeHandler(const CodeBuffer& other):codeBuffer(other){};
    std::string freshVar();
    void handle_binop(const Node& L_exp, const Node& R_exp, Node& new_exp, const std::string op);
    void handle_relop(const Node& L_exp, const Node& R_exp, Node& new_exp, const std::string op);



};;