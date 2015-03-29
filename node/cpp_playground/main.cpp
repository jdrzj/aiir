//
//  main.cpp
//  dic_attack
//
//  Created by Jędrzej Urbański on 22.03.2015.
//  Copyright (c) 2015 Jędrzej Urbański. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <typeinfo>
#include <openssl/sha.h>
#include <bitset>
#include <cmath>
#include "boost/dynamic_bitset.hpp"


void hackify(std::string pass)
{
    //liczba zapisana bitowo
    // 0 - downcase
    // 1 - uppercase

    std::string s;
    std::string pass2;
    int x = pass.length();
    for(int i = 0; i < pow(2, pass.length()); i++)
    {
        pass2 = pass;
        const boost::dynamic_bitset<> b2(x, i);
        boost::to_string(b2, s);
        std::cout << s << std::endl;
        for(int j = 0; j < s.length(); j++)
        {
            //std::cout << s[j] << std::endl;
            if(s[j] == '1')
            {
                pass2[j] = toupper(pass[j]);
            }
            else if (s[j] == '0')
            {
                pass2[j] = tolower(pass[j]);
            }
        }
        std::cout << pass2 << std::endl;
    }

}


int main(int argc, const char * argv[]) {

    std::string password = "lemon";

    std::cout << "Password to hack: " << password << std::endl;
    hackify(password);
    password[0] = toupper (password[0]);
    std::cout << password << std::endl;
    return 0;
}
