#include "llvmcode.hpp"
extern Symtab symtable;


string LlvmCodeHandler::freshVar(){
    return "%var_" + std::to_string(this->RegNum++);
}

string LlvmCodeHandler::globalFreshVar(){
    return "@var_" + std::to_string(this->RegNum++);
}


void LlvmCodeHandler::handle_binop(Node* res_exp, Node* L_exp, Node* R_exp, std::string op){ 
    res_exp->var = freshVar();
    string llvm_op;
    if (op == "*") {
        llvm_op = "mul";
    }
    else if (op == "/") {
        if (res_exp->type == "INT") {
            llvm_op = "sdiv";
        }
        else { //res_exp->type = "byte"
            llvm_op = "udiv";
        }
        // Check devision by zero
        codeBuffer.emit("call void @check_division(i32 " + R_exp->var + ")");
    }
    else if (op == "+") {
        llvm_op = "add";
    }
    else if (op == "-") {
        llvm_op = "sub";
    }
    codeBuffer.emit(res_exp->var + " = " + llvm_op + " i32 " + L_exp->var + ", " + R_exp->var); //var3 add i32 var1, var2

    
    /* Check overflow */
    /* No need for Int_t type since already we use i32 in llvm */
    if (res_exp->type == "BYTE") {
        string new_new_var = freshVar();
        codeBuffer.emit(new_new_var + " = " + "and " + "i32 " + res_exp->var + ", " + "255");
        res_exp->var = new_new_var;
    }


}


void LlvmCodeHandler::handle_relop(BoolVarNode* res_exp, Node* L_exp, Node* R_exp, string op){ 
    string llvm_relop = "";
    bool sign;
    if (op == "=="){
        llvm_relop = "eq";
    }
    else if (op == "!=") {
        llvm_relop = "ne";
    }
    else if (op == "<"){
        llvm_relop = "lt";
    }
    else if (op == ">") {
        llvm_relop = "gt";
    }
    else if (op == "<=" ) {
        llvm_relop = "le";
    }
    else if (op == ">=") {
        llvm_relop = "ge";
    }

    if (L_exp->type == "INT" || R_exp->type == "INT")
        sign = true;
    else
        sign = false;
    
    if (llvm_relop != "eq" && llvm_relop != "ne") {
        if (sign)
            llvm_relop = "s" + llvm_relop;
        else
            llvm_relop = "u" + llvm_relop;
    }

    // std::string relop_start_label = codeBuffer.freshLabel();
    /* nextlist of exp2 is the start of these operations */
    res_exp->var = this->freshVar();
    this->codeBuffer.emit(res_exp->var + " = " + "icmp " + llvm_relop + " i32 " + L_exp->var + ", " + R_exp->var);

    //int next_instr = this->codeBuffer.emit("br i1 " + var + ", label @, label @");

    /* Entry is exp1 */
    //res_exp.start_label = L_exp.start_label;
    /* nextlist of exp1 is exp2 entry*/

}

void LlvmCodeHandler::allocate_var(string type, string id, string assigningVar, string assigningType) {

    string llvmtype = getLlvmType(type);

    string var = freshVar();
    symtable.tableStack.top()->addEntry(id, type, symtable.offsetsStack.top()++, var);
    int offset = symtable.find(id)->offset;

    string ptrvar = freshVar();
    codeBuffer.emit(ptrvar + " = getelementptr i32, i32* " + symtable.rbpvar + ", i32 " + to_string(offset));

    //type id sc
    if(assigningVar == "")
        codeBuffer.emit(var + " = add i32 0, 0");

    //type id assign exp sc
    else {
        if(type == "BOOL")
            codeBuffer.emit(var + " = zext i1 " + assigningVar + " to i32");
        else if(type == "BYTE")
            codeBuffer.emit(var + " = zext i8 " + assigningVar + " to i32");
        else if(type == "INT"){
            if(assigningType == "BYTE")
                codeBuffer.emit(var + " = zext i8 " + assigningVar + " to i32");
            else var = assigningVar;
        }
    }
        codeBuffer.emit("store i32 " + var + ", i32* " + ptrvar);
}

//id assign exp sc
void LlvmCodeHandler::change_var_value(string id, string assigningVar, string assigningType){

    string var = freshVar();
    string ptrvar = freshVar();
    int offest = symtable.find(id)->offset;
    codeBuffer.emit(ptrvar + " = getelementptr i32, i32* " + symtable.rbpvar + ", i32 " + to_string(offest));

    if(assigningVar == "BOOL")
            codeBuffer.emit(var + " = zext i1 " + assigningVar + " to i32");
    else if(assigningType == "BYTE")
            codeBuffer.emit(var + " = zext i8 " + assigningVar + " to i32");
    else if(assigningType == "INT"){
        if(assigningType == "BYTE")
            codeBuffer.emit(var + " = zext i8 " + assigningVar + " to i32");
        else var = assigningVar;
    }

    codeBuffer.emit("store i32 " + var + ", i32* " + ptrvar);
}



string LlvmCodeHandler::getLlvmType(string type){
    string llvmtype;
    if(type == "INT")
        llvmtype = "i32";
    else if(type == "BYTE")
        llvmtype = "i8";
    else if(type == "BOOL")
        llvmtype = "i1";
    else //string
        llvmtype = "i8*";

    return llvmtype;
}

void LlvmCodeHandler::startingPrints() {

    codeBuffer.emit("@.DIV_BY_ZERO_ERROR = internal constant [23 x i8] c\"Error division by zero\\00\"");
    codeBuffer.emit("define void @check_division(i32) {");
    codeBuffer.emit("%valid = icmp eq i32 %0, 0");
    codeBuffer.emit("br i1 %valid, label %ILLEGAL, label %LEGAL");
    codeBuffer.emit("ILLEGAL:");
    codeBuffer.emit("call void @print(i8* getelementptr([23 x i8], [23 x i8]* @.DIV_BY_ZERO_ERROR, i32 0, i32 0))");
    codeBuffer.emit("call void @exit(i32 0)");
    codeBuffer.emit("ret void");
    codeBuffer.emit("LEGAL:");
    codeBuffer.emit("ret void");
    codeBuffer.emit("}");

    codeBuffer.emit("declare i32 @printf(i8*, ...)");
    codeBuffer.emit("declare void @exit(i32)");
    codeBuffer.emit("declare i32 @scanf(i8*, ...)");
    codeBuffer.emit("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
    codeBuffer.emit("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");


    codeBuffer.emit("define void @print(i8*){");
    codeBuffer.emit("call i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
    codeBuffer.emit("ret void");
    codeBuffer.emit("}");

    codeBuffer.emit("define void @printi(i32){");
    codeBuffer.emit("%format_ptr = getelementptr [4 x i8], [4 x i8]* @.intFormat, i32 0, i32 0");
    codeBuffer.emit("call i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.intFormat, i32 0, i32 0), i32 %0)");
    codeBuffer.emit("ret void");
    codeBuffer.emit("}");

    codeBuffer.emit("define i32 @readi(i32) {");
    codeBuffer.emit("%ret_val = alloca i32");
    codeBuffer.emit("%spec_ptr = getelementptr [3 x i8], [3 x i8]* @.int_specifier_scan, i32 0, i32 0");
    codeBuffer.emit("call i32 (i8*, ...) @scanf(i8* %spec_ptr, i32* %ret_val)");
    codeBuffer.emit("%val = load i32, i32* %ret_val");
    codeBuffer.emit("ret i32 %val");
    codeBuffer.emit("}");
}