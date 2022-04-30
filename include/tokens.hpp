#include <iostream>
#include <vector> 
#include <map>
#include <sys/stat.h>

#ifdef WIN32
   std::string os="win32";
#endif
#if __linux__
    std::string os="linux";
#endif
    #if __APPLE__
    std::string os="macos";
#endif

std::string readFile(std::string file){
    const int sizebuf=1234;
    char buff[sizebuf];
    FILE*  fp = fopen (file.data(), "r");
    if (fp== NULL){ return "";}
    fread(buff,sizeof(buff),1,fp);  std::string cur_string(buff);
    fclose(fp); 
    return cur_string;
}

std::vector<std::string> acmds;
enum token{src, arg, lib, out, gxx, nocomp};

struct Token{token t; std::string data;};

std::string file = readFile("./RMakefile");
std::map<std::string,std::string> vars = {{"SOURCE",""},{"ARGS",""},{"LIBS",""},{"GXX",""},{"OUTPUT",""}};

void fillVars(int start=0){
    std::string col; int j;
    for (int i=start; i < file.size(); i++){
        if (file.at(i) == '$'){
            if (file.at(i+1) == '('){
                col=""; j=i+1; 
                while ( j >= 0 && file.at(j) != ')' && file.at(j) != '\n'){ j++; if (file.at(j) != ' ' && file.at(j) != ')' && file.at(j) != '\n') col += file.at(j);}      
                int j2=j; j++;
                if (vars.find(col) != vars.end()){
                    for (int b=0; b < vars.at(col).size(); b++) file.insert(file.begin()+j+b,vars.at(col).at(b));
                }
                bool done=false;
                while (j2-1 >= 0 && !done){ if (file.at(j2) == '$') done=true; file.erase(file.begin()+j2); j2--;}
            } break;   
        }
    }
}

std::vector<Token> tokenize(std::vector<std::string> calls){
    std::vector<Token> output;
    int j; std::string col; bool ta=false; Token t;
    for (int i=0; i < file.size(); i++){
        switch(file.at(i)){
            case '/': 
                if (file.size() > i+1 && file.at(i+1) == '/'){while (i < file.size() && file.at(i) != '\n') i++;} 
                else if (file.size() > i+1 && file.at(i+1) == '*'  &&  i == 0 || file.at(i-1) == '\n'){while (i+1 < file.size() && file.at(i)+file.at(i+1)+"" != "*/") i++;}
                break;
            case '=':
                if (i >= 1 && file.at(i-1) == '+'){
                    j=i-1; col="";
                    while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                    
                    if (col == "SOURCE") t = {src};
                    else if (col == "ARGS") t = {arg};
                    else if (col == "LIBS") t = {lib};

                    if (col == "SOURCE" || col == "ARGS" || col == "LIBS"){
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if ( file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n') t.data += file.at(j);}
                        output.insert(output.end(),t); 
                        if (vars.at(col).size()) vars.at(col) += " ";
                        vars.at(col) += t.data; t={}; col="";
                    } 
                    else if (col == "GXX" || col == "OUTPUT"){
                        std::cout << "You cannot append to GXX or OUTPUT, did you mean to put = instead of += ?" << std::endl;
                    }
                    else if (col != ""){
                        std::string val;
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n') val += file.at(j);}                       
                        if (vars.find(col) != vars.end()){ vars.at(col) += val; col = ""; val=""; vars.insert({col,val}); val= ""; col = ""; fillVars(j);}
                        else std::cout << "Var " << col << " is appened to before it's defined" << std::endl;
                    }
                }
                else if (i >= 1 && file.at(i-1) == '~'){
                    j=i; col="";
                    while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                    std::string col2=""; if (col.size() > 0) for (int i4=0; i4 < col.size()-1; i4++) col2+=col.at(i4); col=col2;
                    std::string val;
                    for (int i3=0; i3 < calls.size(); i3++){
                        if (calls.at(i3).at(0) == '-'){
                            std::string data=""; int j3=1;
                            for (j3; j3 < calls.at(i3).size(); j3++){
                                if (calls.at(i3).at(j3) == '=') break;
                                data += calls.at(i3).at(j3); 
                            } 
                            if (data == col){
                                for (j3+=1; j3 < calls.at(i3).size(); j3++) val += calls.at(i3).at(j3); 
                            }
                        }
                    }
                    if (col == "GXX"){ t = {gxx};
                        t.data=val;
                        if (val == ""){
                            j=i; 
                            while ( j >= 0 && file.at(j) != '\n'){ j++; if ( file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n') t.data += file.at(j);}
                            output.insert(output.end(),t);
                        }
                        vars.at(col) = t.data; t={}; col="";
                    } 
                    else if (col == "SOURCE" || col == "ARGS" || col == "LIBS" || col == "OUTPUT"){
                        std::cout << "You cannot set SOURCE, ARGS, LIBS or OUTPUT as argument vars, did you mean to put = or += ?" << std::endl;
                    }
                    else if (col != ""){
                        if (val == ""){
                            j=i; 
                            while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n') val += file.at(j);}                       
                        }
                        vars.insert({col,val}); val= ""; col = ""; fillVars(j);
                    }
                }
                
                else if (i >= 1 && file.at(i-1) == '!'){
                    j=i; col="";
                    while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                    std::string col2=""; if (col.size() > 0) for (int i4=0; i4 < col.size()-1; i4++) col2+=col.at(i4); col=col2;
                    std::string val;
                    for (int i3=0; i3 < calls.size(); i3++){
                        if (calls.at(i3).at(0) == '-'){
                            std::string data=""; int j3=1;
                            for (j3; j3 < calls.at(i3).size(); j3++){
                                if (calls.at(i3).at(j3) == '=') break;
                                data += calls.at(i3).at(j3); 
                            } 
                            if (data == col){
                                for (j3+=1; j3 < calls.at(i3).size(); j3++) val += calls.at(i3).at(j3); 
                            }
                        }
                    }
                    if (val == "") std::cout << "An argument defintion is required for the " << col << " var, you can define it by adding `-" << col << "=[val] to the command.`" << std::endl;
                    else if (col == "GXX"){ t = {gxx};
                        t.data=val;
                        vars.at(col) = t.data; t={}; col="";
                    } 
                    else if (col == "SOURCE" || col == "ARGS" || col == "LIBS" || col == "OUTPUT"){
                        std::cout << "You cannot set SOURCE, ARGS, LIBS or OUTPUT as force-argument vars, did you mean to put = or += ?" << std::endl;
                    }
                    else if (col != ""){
                        vars.insert({col,val}); val= ""; col = ""; fillVars(j);
                    }
                }

                else{
                    j=i; col="";
                    while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                    if (col == "GXX") t = {gxx};
                    else if (col == "OUTPUT") t = {out};
                    
                    if (col == "GXX" || col == "OUTPUT"){
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n') t.data += file.at(j);}
                        if (col == "GXX" && t.data == "NO-COMP") t.t=nocomp;
                        output.insert(output.end(),t); vars.at(col) = t.data; t={}; col="";
                    } 
                    else if (col == "ARGS" || col == "SOURCE" || col == "LIBS"){
                        std::cout << "You cannot define SOURCE, LIBS or ARGS, did you mean to put += instead of = ?" << std::endl;
                    }
                    else if (col != ""){
                        std::string val;
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n') val += file.at(j);}                       
                        vars.insert({col,val}); val= ""; col = ""; 
                        fillVars(j);
                    }

                } col=""; t={}; break;

            case '{': 
                j=i; col="";
                while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) == ' ' && t.data.size() || file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                if (col.at(col.size()-1) == ')'){
                    std::string col2="";  std::string arg="";  bool nc=true;
                    for (int j3=0; j3 < col.size(); j3++){
                        if (!nc){
                            if (col.at(j3) == ')') break;
                            arg += col.at(j3);
                        }
                        if (col.at(j3) == '(' || col.at(j3) == ' ' || col.at(j3) == '\n') nc=false;
                        if (nc) col2+=col.at(j3);
                    }
                    if (col2 == "!path" || col2 == "path"){
                        bool run=false;
                        struct stat buffer;
                        if (stat (arg.c_str(), &buffer) == 0 && col2 == "path") run=true;
                        if (!(stat (arg.c_str(), &buffer) == 0) && col2 == "!path") run=true;
                        if (run){
                            fillVars(j);
                            col = ""; int j2=i+1;
                            while (j2 < file.size() && file.at(j2) != '}'){
                                if (file.at(j2) != '\n') col+= file.at(j2);
                                else {system(col.data()); col = "";}
                                j2++;
                            }
                        }
                    }
                }
                if (col == "bash"){
                    fillVars(j);
                    col = ""; int j2=i+1;
                    while (j2 < file.size() && file.at(j2) != '}'){
                        if (file.at(j2) != '\n') col+= file.at(j2);
                        else {system(col.data()); col = "";}
                        j2++;
                    }
                }
                else if (col == "pash"){
                    fillVars(j);
                    col = ""; int j2=i+1;
                    while (j2 < file.size() && file.at(j2) != '}'){
                        if (file.at(j2) != '\n') col+= file.at(j2);
                        else {acmds.insert(acmds.end(),col); col = "";}
                        j2++;
                    }
                }
                else if (col == "linux" || col == "win32" || col == "macos"){
                    if (col != os) while (i < file.size() && file.at(i) != '}') i++;
                }
                else{
                    bool go=false;
                    for (int b=0; b < calls.size(); b++) if (col == calls.at(b)) go=true;
                    if (!go) while (i < file.size() && file.at(i) != '}') i++;
                }
                break; 
            default: break;
        }
    }
    return output;
}