#pragma once 
 
 
class AES 
{ 
public: 
typedef enum ENUM_KeySize_ 
{ 
	BIT128 = 0, 
	BIT192, 
	BIT256 
}ENUM_KEYSIZE; 
public: 
	AES( ENUM_KEYSIZE keysize, BYTE *key ); 
	~AES(void); 
	void Cipher( BYTE *input, BYTE *output ); 
	void InvCipher( BYTE *input, BYTE *output ); 
protected: 
	BYTE *RotWord( BYTE *word ); 
	BYTE *SubWord( BYTE *word ); 
	void AddRoundKey(int round); 
	void SubBytes(); 
	void InvSubBytes(); 
	void ShiftRows(); 
	void InvShiftRows(); 
	void MixColumns(); 
	void InvMixColumns(); 
	 static BYTE gfmultby01(BYTE b) 
    { 
      return b; 
    } 
 
    static BYTE gfmultby02(BYTE b) 
    { 
      if (b < 0x80) 
        return (BYTE)(int)(b <<1); 
      else 
        return (BYTE)( (int)(b << 1) ^ (int)(0x1b) ); 
    } 
 
    static BYTE gfmultby03(BYTE b) 
    { 
      return (BYTE) ( (int)gfmultby02(b) ^ (int)b ); 
    } 
 
    static BYTE gfmultby09(BYTE b) 
    { 
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^ 
                     (int)b ); 
    } 
 
    static BYTE gfmultby0b(BYTE b) 
    { 
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^ 
                     (int)gfmultby02(b) ^ 
                     (int)b ); 
    } 
 
    static BYTE gfmultby0d(BYTE b) 
    { 
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^ 
                     (int)gfmultby02(gfmultby02(b)) ^ 
                     (int)(b) ); 
    } 
 
    static BYTE gfmultby0e(BYTE b) 
    { 
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^ 
                     (int)gfmultby02(gfmultby02(b)) ^ 
                     (int)gfmultby02(b) ); 
    } 
	int Nb; 
	int Nk; 
	int Nr; 
	BYTE *key;// the seed key. size will be 4 * keySize from ctor. 
	typedef struct BYTE4_ 
	{ 
		BYTE w[4]; 
	}BYTE4; 
	BYTE4 *w; 
	LPBYTE State[4]; 
	/* 
    private byte[,] iSbox;  // inverse Substitution box  
    private byte[,] w;      // key schedule array.  
    private byte[,] Rcon;   // Round constants. 
    private byte[,] State;  // State matrix*/ 
 
}; 


