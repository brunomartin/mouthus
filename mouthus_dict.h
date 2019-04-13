#ifndef MOUTHUS_DICT_H
#define MOUTHUS_DICT_H

#include <list>
#include <string>

class MouthusDict
{
public:
    MouthusDict();
    
    int letterCountMin() const;
    int letterCountMax() const;
  
    std::string pickWord(int wordLength) const;
    bool verifyWordExistence(std::string word) const;

private:

};

#endif //MOUTHUS_DICT_H