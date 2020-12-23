/* urldecode.cpp */

#include "urldecode.h"
#include "string.h"
#include "stdio.h"
#include "afxwin.h"
#include "StdAfx.h"

bool URL_GBK_Decode(const char *encd, char* decd)
{
    if( encd == 0 )
        return false;

    int i, j=0;
    const char *cd = encd;
    char p[2];
    unsigned int num;
    for( i = 0; i < (int)strlen( cd ); i++ )
    {
        memset( p, '\0', 2 );
        if( cd[i] != '%' )
        {
            decd[j++] = cd[i];
            continue;
        }else if(cd[i] == '%' 
            && ( (cd[i+1]>='0'&&cd[i+1]<='9') ||
            (cd[i+1]>='A'&&cd[i+1]<='F') || 
            (cd[i+1]>='a'&&cd[i+1]<='f') ) )
        {
            p[0] = cd[++i];
            p[1] = cd[++i];
            
            sscanf( p, "%x", &num );
            sprintf(p, "%c", num );
            decd[j++] = p[0];
        }else
        {
            decd[j++] = cd[i];
        }
    }
    decd[j] = '\0';

    return true;
}


bool URL_IS_UTF8(const char* url)
{
    char buffer[4096];
    memset(buffer,0,4096);

    URL_GBK_Decode(url,buffer);

    char* pGBKStr = buffer;
    int   strLen = strlen(pGBKStr);
    
    int nWideByte = 0;
    for( int i=0; i<strLen; i++)
    {
        if( pGBKStr[i] < 0 )
        {
            nWideByte++;
        }
    }

    if( nWideByte == 0 )
    {
        return false;
    }

    int nUtfLen = 
        MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,pGBKStr,-1,NULL, 0);
    if( nUtfLen >0 && (nWideByte%3==0) &&
        (strLen - (2*nWideByte/3)) == (nUtfLen-1) )
    {
        return true;
    }

    return false;
}


bool URL_UTF8_Decode(const char* url,char* decd)
{
    char buffer[4096];
    memset(buffer,0,4096);
    
    URL_GBK_Decode(url,buffer);
    char* pGBKStr = buffer;

    int   nGbkLen = MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,pGBKStr,-1,NULL, 0);

    if( nGbkLen >0 ) // UTF8 STR
    {
        LPWSTR wStr = new WCHAR[nGbkLen];
        MultiByteToWideChar(CP_UTF8,0,pGBKStr, -1, wStr, nGbkLen);
        
        int astrLen     = WideCharToMultiByte(CP_ACP, 0, wStr, -1, NULL, 0, NULL, NULL);
        char* converted = new char[astrLen];
        WideCharToMultiByte(CP_ACP, 0, wStr, -1, converted, astrLen, NULL, NULL);
        strcpy(decd,converted);
        
        delete wStr;
        delete converted;
        
        return true;
    }

    return false;
}


bool URL_Decode(const char* url,char* pStr)
{
    int   nWideByte = 0;
    char* pGBKStr   = NULL;
    
    char buffer[4096];
    memset(buffer,0,4096);
    
    URL_GBK_Decode(url,buffer);
    pGBKStr    = buffer;
    int strLen = strlen(pGBKStr);
    
    for( int i=0; i<strLen; i++)
    {
        if( pGBKStr[i] < 0 )
        {
            nWideByte++;
        }
    }
    
    int nGbkLen = 
        MultiByteToWideChar(CP_ACP,MB_ERR_INVALID_CHARS,pGBKStr,-1,NULL, 0);
    
    if( nGbkLen >0 && (strLen - (nWideByte/2)) == (nGbkLen-1) ) // GBK STR
    {
        strcpy(pStr,pGBKStr);
    }
    
    nGbkLen = MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,pGBKStr,-1,NULL, 0);
    if( nGbkLen >0 &&
        (nWideByte%3==0) &&
        (strLen - (2*nWideByte/3)) == (nGbkLen-1))              // UTF8 STR
    {
        LPWSTR wStr = new WCHAR[nGbkLen];
        MultiByteToWideChar(CP_UTF8,0,pGBKStr, -1, wStr, nGbkLen);
        
        int astrLen     = WideCharToMultiByte(CP_ACP, 0, wStr, -1, NULL, 0, NULL, NULL);
        char* converted = new char[astrLen];
        WideCharToMultiByte(CP_ACP, 0, wStr, -1, converted, astrLen, NULL, NULL);
        strcpy(pStr,converted);
        
        delete wStr;
        delete converted;
        
        return true;
    }
    
    strcpy(pStr,pGBKStr);

    return true;
} 
