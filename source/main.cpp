#include "../include/tokens.hpp"


int main(int argc, char** argv){
    std::vector<std::string> calls; bool project=false;

    std::vector<std::string> size; std::string name; bool noRSGL=false; bool postRun=false; bool help=false;
    //RSGLMake -project -size=500,500,500,500 -name=name -noRSGL -postRun
    for (int i=1; i < argc; i++){ 
        std::string str = argv[i];
        if (project){
            std::string five;
            for (int j=0; j < 5 && j < str.size(); j++) five+=str.at(j);
            if (five == "-name"){
                for (int j=6; j  < str.size(); j++){
                    name+=str.at(j);
                }
            }
            if (five == "-size"){
                std::string num;
                for (int j=6; j  < str.size(); j++){
                    num += str.at(j);
                    if (j ==  str.size()-1 || str.at(j) == 'x'){
                        size.insert(size.end(),std::to_string(std::stoi(num))); num="";
                        if (j ==  str.size()-1) break;
                    }
                }
            }  
            if (str == "-noRSGL") noRSGL=true;
            if (str == "-postRun") postRun=true;
            if (str == "-help"){
                std::cout << "-name : the name of the project (required)\n-size : the size of the window (required for an RSGL project)\n-noRSGL : if the project is not an RSGL project\n-postRun : should the program auto run after you compile?\n-help : outputs al the arguments" << std::endl;
                help=true; break;
            }
        }
        if (str == "-project") project = true;
        if (!project) calls.insert(calls.end(),str);
    }
    if (!project){
        if (file == ""){ std::cout << "No RMakefile file found or empty file found" << std::endl; return 0;}
        std::vector<Token> tokens = tokenize(calls);
        bool comp=true;
        std::string source, args, libs, output, compiler;
        for (int i=0; i < tokens.size() && comp; i++){
            switch(tokens.at(i).t){
                case src: source += tokens.at(i).data + " "; break;
                case arg: args += tokens.at(i).data + " ";  break;
                case lib: libs += tokens.at(i).data + " "; break;
                case out: output += tokens.at(i).data + " "; break;
                case gxx: compiler += tokens.at(i).data + " "; break;
                case nocomp: comp=false; break;
            } 
        }
        if (comp){
            std::string cmd = compiler + source + args + libs + "-o " + output;
            system(cmd.data());
        }
        for (int i=0; i < acmds.size(); i++) system(acmds.at(i).data());
    } else if (!help){
        std::string cmd = "mkdir " + name + " " + name + "/source " + name + "/include " + name + "/res"; system(cmd.c_str());
        std::string file = name + "/source/main.cpp"; FILE* f = fopen(file.c_str(),"w+");
        
        std::string file2 = name + "/RMakefile"; FILE* f2 = fopen(file2.c_str(),"w+");
        std::string data="",data2="SOURCE += source/*.cpp\nARGS += -g\nOUTPUT = " + name + "\nGXX = g++";
        if (!noRSGL) data2="SOURCE += source/*.cpp\nLIBS += -lRSGL\nARGS += -g\nOUTPUT = " + name + "\nGXX = g++" ; 
        if (postRun) data2 += "\n\npash{\n      ./$(OUTPUT)\n}"; 
        fwrite(data2.data(),1,data2.size(),f2); fclose(f2); 
        if (noRSGL){ 
            data = "#include <iostream>\n\nint main(){\n}";
        }
        else{
            data = "#include <RSGL/RSGL.hpp>\n#include <iostream>\n\nRSGL::window win(\""+name+"\",{500,500,"+size.at(0)+","+size.at(1)+"},{255,255,255});\nbool running=true; \n\n";
            data += "int main(){\n      while(running){\n            win.checkEvents();\n            switch(win.event.type){\n                 case RSGL::quit: running=false; break;\n                 default: break;";
            data += "\n            }\n\n            win.clear();\n      } win.close();\n}";
        } fwrite(data.data(),1,data.size(),f); fclose(f); 
    }
} 