#include <iostream>
#include <vector> 
#include <map>

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

enum token{src, arg, lib, out, gxx};

struct Token{token t; std::string data;};

std::string file = readFile("./RMakefile");
std::map<std::string,std::string> vars;

void fillVars(int start=0){
    std::string col; int j;
    for (int i=start; i < file.size(); i++){
        if (file.at(i) == '$'){
            if (file.at(i+1) == '('){
                col=""; j=i+1; 
                while ( j >= 0 && file.at(j) != ')' && file.at(j) != '\n'){ j++; if (file.at(j) != ' ' && file.at(j) != ')' && file.at(j) != '\n') col += file.at(j);}      
                int j2=j; j++;
                for (int b=0; b < vars.at(col).size(); b++) file.insert(file.begin()+j+b,vars.at(col).at(b));
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
                    while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                    
                    if (col == "SOURCE") t = {src};
                    else if (col == "ARGS") t = {arg};
                    else if (col == "LIBS") t = {lib};

                    if (col == "SOURCE" || col == "ARGS" || col == "LIBS"){
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) != ' ' && file.at(j) != '\n') t.data += file.at(j);}
                        output.insert(output.end(),t); t={}; col="";
                    } 
                    else if (col != ""){
                        std::string val;
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) != ' ' && file.at(j) != '\n') val += file.at(j);}                       
                        if (vars.find(col) != vars.end()){ vars.at(col) += val; col = ""; val=""; vars.insert({col,val}); val= ""; col = ""; fillVars(j);}
                        else std::cout << "Var " << col << " is appened to before it's defined" << std::endl;
                    }
                }
                else{
                    j=i; col="";
                    while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                    
                    if (col == "GXX") t = {gxx};
                    else if (col == "OUTPUT") t = {out};
                    
                    if (col == "GXX" || col == "OUTPUT"){
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) != ' ' && file.at(j) != '\n') t.data += file.at(j);}
                        output.insert(output.end(),t); t={}; col="";
                    } 
                    else if (col != ""){
                        std::string val;
                        j=i; 
                        while ( j >= 0 && file.at(j) != '\n'){ j++; if (file.at(j) != ' ' && file.at(j) != '\n') val += file.at(j);}                       
                        vars.insert({col,val}); val= ""; col = ""; fillVars(j);
                    }

                } col=""; t={}; break;

            case '{': 
                j=i; col="";
                while ( j-1 >= 0 && file.at(j) != '\n'){ j--; if (file.at(j) != ' ' && file.at(j) != '\n' ) col.insert(col.begin(),file.at(j));}
                if (col == "bash"){
                    col = ""; int j2=i+1;
                    while (j2 < file.size() && file.at(j2) != '}'){
                        if (file.at(j2) != '\n') col+= file.at(j2);
                        else {system(col.data()); col = "";}
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