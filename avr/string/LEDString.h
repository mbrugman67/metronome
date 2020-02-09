#ifndef STRING_H_
#define STRING_H_

class LEDString
{
public:
    LEDString* getInstance();
    
private:
    LEDString() {}
    virtual ~LEDString() {}

    static LEDString* _inst;
};

#endif // STRING_H_