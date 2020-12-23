#include "StdAfx.h"    
#include ".\aes.h"    
const BYTE Sbox[16][16] = {  // populate the Sbox matrix    
    /* 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */   
    /*0*/  {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},   
    /*1*/  {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},   
    /*2*/  {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},   
    /*3*/  {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},   
    /*4*/  {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},   
    /*5*/  {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},   
    /*6*/  {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},   
    /*7*/  {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},   
    /*8*/  {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},   
    /*9*/  {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},   
    /*a*/  {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},   
    /*b*/  {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},   
    /*c*/  {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},   
    /*d*/  {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},   
    /*e*/  {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},   
    /*f*/  {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}    
};   

const BYTE iSbox[16][16] = {  // populate the iSbox matrix    
    /* 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */   
    /*0*/  {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},   
    /*1*/  {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},   
    /*2*/  {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},   
    /*3*/  {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},   
    /*4*/  {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},   
    /*5*/  {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},   
    /*6*/  {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},   
    /*7*/  {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},   
    /*8*/  {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},   
    /*9*/  {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},   
    /*a*/  {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},   
    /*b*/  {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},   
    /*c*/  {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},   
    /*d*/  {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},   
    /*e*/  {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},   
    /*f*/  {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d} };   
    const BYTE Rcon[11][4] = { {0x00, 0x00, 0x00, 0x00},     
    {0x01, 0x00, 0x00, 0x00},   
    {0x02, 0x00, 0x00, 0x00},   
    {0x04, 0x00, 0x00, 0x00},   
    {0x08, 0x00, 0x00, 0x00},   
    {0x10, 0x00, 0x00, 0x00},   
    {0x20, 0x00, 0x00, 0x00},   
    {0x40, 0x00, 0x00, 0x00},   
    {0x80, 0x00, 0x00, 0x00},   
    {0x1b, 0x00, 0x00, 0x00},   
    {0x36, 0x00, 0x00, 0x00} };   
    BYTE *AES::RotWord( BYTE *word )   
    {   
        BYTE *result = new BYTE [4];   
        result[0] = word[1];   
        result[1] = word[2];   
        result[2] = word[3];   
        result[3] = word[0];   
        delete[] word;   
        return result;   
    };   
    BYTE *AES::SubWord( BYTE *word )   
    {   
        BYTE *result = new BYTE[4];   
        result[0] = Sbox[ word[0] >> 4][ word[0] & 0x0f ];   
        result[1] = Sbox[ word[1] >> 4][ word[1] & 0x0f ];   
        result[2] = Sbox[ word[2] >> 4][ word[2] & 0x0f ];   
        result[3] = Sbox[ word[3] >> 4][ word[3] & 0x0f ];   
        delete[] word;   
        return result;   
    }   
    AES::AES( ENUM_KEYSIZE keysize, BYTE *key )   
    {   
		
        this->Nb = 4;   
        switch( keysize )   
        {   
        case AES::BIT128:   
            this->Nk = 4;   
            this->Nr = 10;   
            break;   
        case AES::BIT192:   
            this->Nk = 6;   
            this->Nr = 12;   
            break;   
        case AES::BIT256:   
        default:   
            this->Nk = 8;   
            this->Nr = 14;   
            break;   
        }   
        this->key = new BYTE[this->Nk * 4];   
        memcpy( this->key, key, this->Nk * 4 );   
        this->w = new BYTE4[Nb * (Nr+1)];   
        {for( int row = 0; row < Nk; ++row )   
        {   
            w[row].w[0] = this->key[4*row];   
            w[row].w[1] = this->key[4*row+1];   
            w[row].w[2] = this->key[4*row+2];   
            w[row].w[3] = this->key[4*row+3];   
        }}   
        BYTE *temp = new BYTE[4];   
        {for( int row = Nk; row < Nb *(Nr + 1); ++ row )   
        {   
            temp[0] = this->w[row-1].w[0];   
            temp[1] = this->w[row-1].w[1];   
            temp[2] = this->w[row-1].w[2];   
            temp[3] = this->w[row-1].w[3];   
            if (row % Nk == 0)   
            {   
                temp = SubWord(RotWord(temp));//this change two size    
                temp[0] = (BYTE)( (int)temp[0] ^ (int)Rcon[row/Nk][0] );   
                temp[1] = (BYTE)( (int)temp[1] ^ (int)Rcon[row/Nk][1] );   
                temp[2] = (BYTE)( (int)temp[2] ^ (int)Rcon[row/Nk][2] );   
                temp[3] = (BYTE)( (int)temp[3] ^ (int)Rcon[row/Nk][3] );   
            }   
            else if ( Nk > 6 && (row % Nk == 4) )     
            {   
                temp = SubWord(temp);   
            }   
			
            // w[row] = w[row-Nk] xor temp    
            this->w[row].w[0] = (BYTE) ( (int)this->w[row-Nk].w[0] ^ (int)temp[0] );   
            this->w[row].w[1] = (BYTE) ( (int)this->w[row-Nk].w[1] ^ (int)temp[1] );   
            this->w[row].w[2] = (BYTE) ( (int)this->w[row-Nk].w[2] ^ (int)temp[2] );   
            this->w[row].w[3] = (BYTE) ( (int)this->w[row-Nk].w[3] ^ (int)temp[3] );   
        }//loop    
		}
        delete[] temp;   
        for(int i=0;i<4;i++)   
        {   
            this->State[i] = NULL;   
        }   
    }   
	
    AES::~AES(void)   
    {   
        for(int i=0;i<4;i++)   
        {   
            if (this->State[i] != NULL)   
            {   
                delete []this->State[i];   
            }   
        }   
    }   
    void AES::Cipher( BYTE *input, BYTE *output )   
    {   
        if (this->State[0] == NULL)   
        {   
            for(int i=0;i<4;i++)   
            {   
                this->State[i] = new BYTE[this->Nb];   
            }   
        }   
		
		{
			for (int i = 0; i < (4 * Nb); ++i)   
			{   
				this->State[i%4][i/4] = input[i];   
			}
		}
        AddRoundKey(0);   
		
        for (int round = 1; round <= (Nr - 1); ++round)  // main round loop    
        {   
            SubBytes();    
            ShiftRows();     
            MixColumns();    
            AddRoundKey(round);   
        }  // main round loop    
		
        SubBytes();   
        ShiftRows();   
        AddRoundKey(Nr);   
		
        // output = state    
        for (int i = 0; i < (4 * Nb); ++i)   
        {   
            output[i] = this->State[i % 4][ i / 4];   
        }   
		
    }   
    void AES::InvCipher( BYTE *input, BYTE *output )   
    {   
        if (this->State[0] == NULL)   
        {   
            for(int i=0;i<4;i++)   
            {   
                this->State[i] = new BYTE[this->Nb];   
            }   
        }   
		
		{
			for (int i = 0; i < (4 * Nb); ++i)   
			{   
				this->State[i % 4][ i / 4] = input[i];   
			}   
		}
		
        AddRoundKey(Nr);   
		
        for (int round = Nr-1; round >= 1; --round)  // main round loop    
        {   
            InvShiftRows();   
            InvSubBytes();   
            AddRoundKey(round);   
            InvMixColumns();   
        }  // end main round loop for InvCipher    
		
        InvShiftRows();   
        InvSubBytes();   
        AddRoundKey(0);   
		
        // output = state    
        for (int i = 0; i < (4 * Nb); ++i)   
        {   
            output[i] = this->State[i % 4][ i / 4];   
        }   
    }   
    void AES::AddRoundKey( int round )   
    {   
        for (int r = 0; r < 4; ++r)   
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                this->State[r][c] = (BYTE) ( (int)this->State[r][c] ^ (int)w[(round*4)+c].w[r] );   
            }   
        }   
    }   
	
    void AES::SubBytes()   
    {   
        for (int r = 0; r < 4; ++r)   
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                this->State[r][c] = Sbox[ (this->State[r][c] >> 4)][ (this->State[r][c] & 0x0f) ];   
            }   
        }   
    }   
    void AES::InvSubBytes()   
    {   
        for (int r = 0; r < 4; ++r)   
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                this->State[r][c] = iSbox[ (this->State[r][c] >> 4)][ (this->State[r][c] & 0x0f) ];   
            }   
        }   
    }   
    void AES::ShiftRows()   
    {   
        BYTE4 temp[4];   
        //  byte[,] temp = new byte[4,4];   
		{
			for (int r = 0; r < 4; ++r)  // copy State into temp[]    
			{   
				for (int c = 0; c < 4; ++c)   
				{   
					temp[r].w[c] = this->State[r][c];   
					// temp[r,c] = this.State[r,c];    
				}   
			}
		}
		
        for (int r = 1; r < 4; ++r)  // shift temp into State    
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                this->State[r][c] = temp[ r].w[ (c + r) % Nb ];   
            }   
        }   
    }  // ShiftRows()    
    void AES::InvShiftRows()   
    {   
        BYTE4 temp[4];   
        {for (int r = 0; r < 4; ++r)  // copy State into temp[]    
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                temp[r].w[c] = this->State[r][c];   
            }   
        }   }
		
        for (int r = 1; r < 4; ++r)  // shift temp into State    
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                this->State[r][ (c + r) % Nb ] = temp[r].w[c];   
            }   
        }   
    }  // InvShiftRows()    
    void AES::MixColumns()   
    {   
        BYTE4 temp[4];   
        for (int r = 0; r < 4; ++r)  // copy State into temp[]    
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                temp[r].w[c] = this->State[r][c];   
            }   
        }   
		
        for (int c = 0; c < 4; ++c)   
        {   
            this->State[0][c] = (BYTE) ( (int)gfmultby02(temp[0].w[c]) ^ (int)gfmultby03(temp[1].w[c]) ^   
                (int)gfmultby01(temp[2].w[c]) ^ (int)gfmultby01(temp[3].w[c]) );   
            this->State[1][c] = (BYTE) ( (int)gfmultby01(temp[0].w[c]) ^ (int)gfmultby02(temp[1].w[c]) ^   
                (int)gfmultby03(temp[2].w[c]) ^ (int)gfmultby01(temp[3].w[c]) );   
            this->State[2][c] = (BYTE) ( (int)gfmultby01(temp[0].w[c]) ^ (int)gfmultby01(temp[1].w[c]) ^   
                (int)gfmultby02(temp[2].w[c]) ^ (int)gfmultby03(temp[3].w[c]) );   
            this->State[3][c] = (BYTE) ( (int)gfmultby03(temp[0].w[c]) ^ (int)gfmultby01(temp[1].w[c]) ^   
                (int)gfmultby01(temp[2].w[c]) ^ (int)gfmultby02(temp[3].w[c]) );   
        }   
    }  // MixColumns    
    void AES::InvMixColumns()   
    {   
        BYTE4 temp[4];   
        for (int r = 0; r < 4; ++r)  // copy State into temp[]    
        {   
            for (int c = 0; c < 4; ++c)   
            {   
                temp[r].w[c] = this->State[r][c];   
            }   
        }   
		
        for (int c = 0; c < 4; ++c)   
        {   
            this->State[0][c] = (BYTE) ( (int)gfmultby0e(temp[0].w[c]) ^ (int)gfmultby0b(temp[1].w[c]) ^   
                (int)gfmultby0d(temp[2].w[c]) ^ (int)gfmultby09(temp[3].w[c]) );   
            this->State[1][c] = (BYTE) ( (int)gfmultby09(temp[0].w[c]) ^ (int)gfmultby0e(temp[1].w[c]) ^   
                (int)gfmultby0b(temp[2].w[c]) ^ (int)gfmultby0d(temp[3].w[c]) );   
            this->State[2][c] = (BYTE) ( (int)gfmultby0d(temp[0].w[c]) ^ (int)gfmultby09(temp[1].w[c]) ^   
                (int)gfmultby0e(temp[2].w[c]) ^ (int)gfmultby0b(temp[3].w[c]) );   
            this->State[3][c] = (BYTE) ( (int)gfmultby0b(temp[0].w[c]) ^ (int)gfmultby0d(temp[1].w[c]) ^   
                (int)gfmultby09(temp[2].w[c]) ^ (int)gfmultby0e(temp[3].w[c]) );   
        }   
    }  
	
