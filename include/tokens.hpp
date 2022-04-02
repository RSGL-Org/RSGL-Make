#include <iostream>
#include <vector> 
#include <map>

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

std::vector<Token> tokenize(){
    std::vector<Token> output; std::map<std::string,std::string> vars = {{"a","a"}};
    std::string file = readFile("./RMakefile");
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
                        if (vars.find(col) != vars.end()){ vars.at(col) += val; col = ""; val="";}
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
                        vars.insert({col,val}); val= ""; col = "";
                    }

                } col=""; t={}; break;
            /*case '$':
                if (file.at(i+1) == '('){

                } break;
            case ':':
                if (file.at(i-5) + file.at(i-4) + file.at(i-3) + file.at(i-2) + file.at(i-1)+"" == "bash"){

                } break;*/
            case '{': break; 
            default: break;
        }
    }
    return output;
}
