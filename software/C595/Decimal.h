// simple Decimal Floating Point library
// 9 significant digits
//
#include <Arduino.h>
#include <limits.h>

struct Decimal : public Printable 
{
   static long power10[];
 
   bool hasDp = false;
   uint32_t value = 0;
   int exp = 0;
   int sign = 1;
   bool resetOnUpdate = false;
  // char disp[20];
   bool error = false;
   Decimal() : value(0), exp(0) { }
   Decimal(long v, int e) : value(abs(v)), exp(e) { sign = v < 0 ? -1 : 1;}
   
   size_t printTo(Print& p) const 
   {
       size_t r = 0;

       r += p.print("[");    
       r += p.print(sign == 1 ? "" : "-");
       r += p.print(value);
       r += p.print(",");
       r += p.print(exp);
       r += p.print("]");
       
       return r;      
   }
       
   void clear()
   { 
      value = 0;
      exp = 0;
      hasDp = false;
      resetOnUpdate = false;
      sign = 1;
      error=false;
   }

   void setValue( long v)
   {
       if( v < 0)
       {
           value = abs(v);
           sign = -1;
       }
       else
       {
           value = v;
           sign = 1;
       }
   }
   
   Decimal& setVal( char key)
   {
    error = false;
    
     if(key == '-')
     {
        sign = -1;
        hasDp = false;     
        exp = 0;
        value = 0;
     }
     else if(key == '.')
     {
        hasDp = true;
        exp = 0;
        value = 0;
     }
     else
     {
        hasDp = false;
        exp = 0;
        value = key - '0';
     }
  
     return *this;
  }

  Decimal& updateVal(char key)
  {
     if(resetOnUpdate)
     {
       resetOnUpdate =false;
       return setVal(key);
     }
     if(key == '.')
     {
       if(!hasDp)
         hasDp = true;
     }
     else
     { 
       if(numDigits(value) >=9)
         return *this;
         
       if(hasDp)
         exp--;
       
       value = value * 10 + (key - '0');
     }
  
     return *this;
  }
    
    
   char* format(char* buffer, int maxWidth = 9, bool decimalPointWidth = 0)
   {
      char localBuf[20];
      int width = sign == -1 ? maxWidth -1 : maxWidth; // space for '-'

      strcpy(buffer, sign == -1 ? "-" : "");
         
      if(this->exp >= 0)
      {
          if(numDigits(this->value) + this->exp <= width)
          {
              if(this->exp == 0)
              {
                 strcat(buffer, ltoa(this->value, localBuf, 10));
                 if(this->hasDp)
                     strcat(buffer, ".");
              }
              else
              {
                 strcat(buffer, ltoa(this->value, localBuf, 10));
                 for(int i = 0; i < this->exp; i++)
                    strcat(buffer, "0");
                 if(this->hasDp)
                     strcat(buffer, ".");          
              }
          }
          else
          {
            // we need scientific notation
            Decimal norm = this->normalise();
            
            int expLength = numDigits(norm.value) + norm.exp > 16 ? 3 : 2;
            int digitsNeeded = numDigits(norm.value) - (width - expLength); 
            if (digitsNeeded > 0)
            {
               norm.decimate(digitsNeeded);
            }
            strcat(buffer, ltoa(abs(norm.value), localBuf, 10));
            strcat(buffer, "E");
            strcat(buffer, ltoa(norm.exp, localBuf, 10));
          }
      }
      else
      { 
        // exp < 0
        int absExp = abs(this->exp);
        if(absExp < width - decimalPointWidth)
        {
           if(absExp >= numDigits(this->value))
              strcat(buffer, "0.");
           if(absExp > numDigits(this->value))
           {
               for(int i = 0; i < absExp - numDigits(this->value); i++)
                  strcat(buffer, "0");
           }
           if(absExp >= numDigits(this->value))
              strcat(buffer, ltoa(this->value, localBuf, 10));
              
           if(absExp < numDigits(this->value))
           {
               strcat(buffer, ltoa(this->value/power10[absExp], localBuf, 10));
               strcat(buffer, ".");
               strcat(buffer, ltoa(this->value%power10[absExp], localBuf, 10));
           }
        }
        else
        {
            // use sci notation
            Decimal norm = this->normalise();
 
            int nDigits = numDigits(norm.value);
            strcat(buffer, ltoa(norm.value/power10[nDigits -1], localBuf, 10));
            strcat(buffer, ".");
            ltoa(norm.value%power10[nDigits -1], localBuf, 10);

            int len = width - decimalPointWidth - 3 - numDigits(norm.exp + (nDigits - 1));  

            strncat(buffer, localBuf, len); 
            strcat(buffer, "E");
            strcat(buffer, ltoa(  norm.exp + (nDigits - 1), localBuf, 10)); 
        }
      }

      return buffer;
   }

   
   Decimal& operator+=(const Decimal& rhs)
   {      
      this->error = false;
      auto maxExp = max(this->exp, rhs.exp);
      if(maxExp < 1 || this->exp == rhs.exp)
      {
        this->setValue(this->value * this->sign + rhs.value * rhs.sign);
      }
      else
      {
        if (this->exp < maxExp)
        {
          this->setValue( this->value * power10[maxExp - this->exp] * this->sign + rhs.value * rhs.sign);
          this->exp = maxExp;
        }
        else
        {
          this->setValue( this->value * this->sign + rhs.value * power10[maxExp - rhs.exp] * rhs.sign);
        }              
      }
         
      return *this;
   }
   
   Decimal operator+(const Decimal& rhs)
   {      
       Decimal ret = *this;
       ret += rhs; 

       return ret;
   }
   
   Decimal operator-(const Decimal& rhs)
   {
      Decimal ret;
      auto maxExp = max(this->exp, rhs.exp);
      if(maxExp < 1 || this->exp == rhs.exp)
      {
        ret.setValue( this->value * this->sign - rhs.value * rhs.sign);
      }
      else
      {
        if (this->exp < maxExp)
        {
          ret.setValue( this->value * power10[maxExp - this->exp] * this->sign - rhs.value * rhs.sign);
          ret.exp = maxExp;
        }
        else
        {
          ret.setValue( this->value * this->sign - rhs.value * power10[maxExp - rhs.exp] * rhs.sign);
        }             
      }
         
      return ret;
   }
   
   Decimal operator*(const Decimal& rhs)
   {
      Decimal ret;
       
      if(this->value < LONG_MAX / rhs.value)
      {     
          ret.value = this->value * rhs.value;    
          ret.sign = this->sign * rhs.sign;                                 
          ret.exp = this->exp + rhs.exp;    
      }
      else
      {
         ret = this->normalise();
         Decimal normRhs = rhs.normalise();
    
         while(ret.value > LONG_MAX / normRhs.value)
         {
             ret.value /= 10;
             ret.exp++;
    
             normRhs.value /=10;
             normRhs.exp++;
         }
       
         ret.value = ret.value * normRhs.value; 
         ret.sign = ret.sign * normRhs.sign;                      
         ret.exp = ret.exp + normRhs.exp;      
      }    
    
      return ret;
   }
   
   Decimal operator/(const Decimal& rhs)
   {
       Decimal ret;
       
       int32_t dividend = this->value;
       int32_t divisor = rhs.value; 

       if(dividend == 0)
       {
          ret.value = 0;
          ret.sign = 1; 
          ret.exp = 0;

          return ret;
       }
       
       if(divisor == 0)
       {
         ret.value = 0;
          ret.sign = 1; 
          ret.exp = 0;
          ret.error = true;
          return ret;
       }
             
       if(this->value >= rhs.value && this->value % rhs.value == 0)
       {
          ret.value = this->value / rhs.value; // integral divisor
          ret.sign = this->sign * rhs.sign; 
          ret.exp = this->exp - rhs.exp;   
       }  
       else
       {
          ret = this->normalise();
          Decimal normRhs = rhs.normalise();
 
          int32_t wholePart = ret.value / normRhs.value; 
          int32_t remainder = ret.value - (wholePart * normRhs.value); 
          divisor = normRhs.value;
          
          ret.value = wholePart; 
          ret.exp = ret.exp - normRhs.exp;   
          ret.sign = ret.sign * normRhs.sign; 

          // long division!
          for (int32_t i = 0; i < 9; i++) 
          {
            if(remainder == 0)
              break;
            if(numDigits(ret.value) >= 9)
              break;
              
            remainder *= 10; 
            wholePart = remainder / divisor; 
   
            ret.value = ret.value * 10 + wholePart;
            ret.exp--;
            remainder -= (wholePart * divisor); 
          }         
       }
 
       return ret;
   }

   Decimal normalise () const
   {
       Decimal ret(this->value, this->exp);

       int nDigits = numDigits(ret.value);
       for (int i = 0; i < nDigits; i++) 
       {
         if(ret.value % 10 == 0)
         {
           ret.value /= 10;
           ret.exp++;
         }     
         else
         {
            break;
         }
       } 
  
       return ret;
   }


   Decimal& decimate(int count)
   {   
     for (int i = 0; i < count; i++) 
     {
         this->value /= 10;
         this->exp++;
     } 

     return *this;
   }

   static int numDigits (long n) 
   {
      if (n < 0) n = (n == LONG_MIN) ? LONG_MAX : -n;
      if (n < 10) return 1;
      if (n < 100) return 2;
      if (n < 1000) return 3;
      if (n < 10000) return 4;
      if (n < 100000) return 5;
      if (n < 1000000) return 6;
      if (n < 10000000) return 7;
      if (n < 100000000) return 8;
      if (n < 1000000000) return 9;
     
      return 10;
   }
   
   Decimal percent()
   {     
      Decimal result = *this / Decimal(100,0);
      return result;
   }
};

