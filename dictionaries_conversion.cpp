#include <list>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cassert>

std::string retireAccent(std::string message);

int main(int argc, char* argv[])
{
    std::list<std::string> fileNames;
    
    std::string filesPath = __FILE__;
    
#ifdef WIN32
    std::string nativeSeparator = "\\";
#else
    std::string nativeSeparator = "/";
#endif
    
    filesPath = filesPath.substr(0,filesPath.find_last_of(nativeSeparator)+1);
    
//    std::cout << "filesPath : " << filesPath << std::endl;

//    std::stringstream ssHeaderFile;
//    ssHeaderFile << filesPath << "motus_dict_ressource.h";
//    std::string headerFileName = ssHeaderFile.str();
//
//    std::fstream headerFile(headerFileName.c_str(), std::fstream::out);
//    assert(headerFile.is_open());
//

    std::stringstream ssSourceFile;
    ssSourceFile << filesPath << "motus_dict_ressource.h";
    std::string sourceFileName = ssSourceFile.str();

//    std::cout << "sourceFileName : " << sourceFileName << std::endl;

    std::fstream sourceFile(sourceFileName.c_str(), std::fstream::out);
    assert(sourceFile.is_open());
    
    //    sourceFile << "#include \"motus_dict_ressource.h\"" << std::endl << std::endl;
    
    sourceFile << "// motus_dist_ressource.h" << std::endl << std::endl;
    sourceFile << "// les fichiers texte sont créés à partir des " << std::endl << std::endl;
    sourceFile << "// résultats du site http://www.lexique.org" << std::endl << std::endl;

    sourceFile << "#ifndef MOUTHUS_DICT_RESSOURCE_H" << std::endl;
    sourceFile << "#define MOUTHUS_DICT_RESSOURCE_H" << std::endl << std::endl;    
    sourceFile << "namespace MotusDictRessource {" << std::endl << std::endl;
    
    for(int i=5;i<=13;i++)
    {
        std::stringstream ssText;
        ssText << filesPath << "ressources/liste_mot_" << i << "_lettres.txt";
        std::string textFileName = ssText.str();

//        std::cout << "textFileName : " << textFileName << std::endl;

        std::ifstream textFile(ssText.str().c_str());
        assert(sourceFile.is_open());

        
        std::set<std::string> words;
        
        while(!textFile.eof())
        {
            std::string word;
            std::getline(textFile,word);

			while(word[word.length()-1] == ' ' || word[word.length()-1] == '\t')
				word = word.substr(0,word.length()-1);
                        
            word = retireAccent(word);

            if(word.find("-") != std::string::npos) continue;
            if(word.find(" ") != std::string::npos) continue;
            if(word.find('\'') != std::string::npos) continue;
            
            word = word.substr(0,i);
            
            words.insert(word);
        }

//        headerFile << "\tint wordList" << i << "LettersSize;" << std::endl;
        
//        headerFile << "\tchar* wordList" << i << "Letters[" << words.size() << "];" << std::endl << std::endl;

        sourceFile << "\tconst int wordList" << i << "LettersSize = " << words.size() << ";" << std::endl;

        sourceFile << "\tconst char* wordList" << i << "Letters[] =" << std::endl;
        sourceFile << "\t{" << std::endl;

        for(std::set<std::string>::iterator iter=words.begin();iter!=words.end();iter++)
        {
            std::string word = *iter;
            sourceFile << "\t\t\"" << word << "\"," << std::endl;
        }
        
        textFile.close();
        sourceFile << "\t};" << std::endl << std::endl;
        
        std::cout << words.size() << " mots à " << i << " lettres rajoutés au fichier ressource" << std::endl << std::endl;
    }
    
    sourceFile << "};" << std::endl << std::endl;
    sourceFile << "#endif //MOUTHUS_DICT_RESSOURCE_H" << std::endl;

    sourceFile.close();
//    headerFile.close();

}

std::string retireAccent(std::string message)
{
    std::string accent("ÀÁÂÃÄÅàáâãäåÒÓÔÕÖØòóôõöøÈÉÊËèéêëÌÍÎÏìíîïÙÚÛÜùúûüÿÑñÇç");
    std::string sansAccent("AAAAAAaaaaaaOOOOOOooooooEEEEeeeeIIIIiiiiUUUUuuuuyNnCc");
    std::string messageCourt;
    int i=0,j=0,k=0,taille;
    
    taille=message.length();
    
    for (i=0;i<taille;i++)
    {
        for(j=0;j<=sansAccent.size();j=j+2)
        {
            if(message[i]==accent[j])
				if(message[i+1]==accent[j+1])
				{
					message[i]=sansAccent[j/2];
					for(k=i+1;k<taille-1;k++)
					{
						message[k]=message[k+1];
					}
					message=message.substr(0,taille-1);
					taille=message.size();
				}
        }
    }
    
    return message;
}
