# include <iostream>    
# include <string>       
# include <vector>          
# include <cstdlib>     
# include <iomanip>    
# include <cstring>           
# include <ctype.h>
# include <sstream>
# include <stdio.h>

using namespace std;

int gline = 1 ;
int gcol = 1 ;
int gline_before = 1 ;
int gcol_before = 1 ;
bool gonly_one = false ; // ���J�u���@�Ӯɳ]��true 
bool gend_of_file = false ;
bool gSexpDone = false ; // �ˬdSexp�����S �Φbgettoken ";" 
bool gPrintDone = false ; // �ˬd���@�榳�L�L�X ���L�X�N���@��w�g���� �U�@��line����++ 
bool gIs_Define = false ; // ���F�Φbdefine �٥Φbclean environment�W �u�O���Fclean���ᤣ�n�L�X�� ���s�Aloop�@�� 
bool gIsNew_tree = false ; // �ˬd�O�_�i�Jwhich �o��function �ΨӰϤ� 1 �M car '( 1 2 3 ) �Ϊ� 
bool gpro = true ;
bool gProPrint = false ; // if ���F���ɭ� �̭���func����Lpro 
bool gnon = false ;
bool gattempt = false ; // �ΨӬݬO���Oattempt error
bool gproLambda = false ;
bool gDef = false ; // �ݦ��S���Q�w�q ���htrue �u�O�g�� �צӤw 
bool gWhite = false ;
int gfirst_which = 0 ; // �ΨӬ�clean-environ �O���O��W�� �j��1�N���O��W�� ( clean ... ) �~�O���T 
string gErrorStr = "" ; // �ΨӦs���~������token 

string g_speek = "" ;
bool g_bpeek = false ;

struct Token {
  string curstr ; // return token
  vector<string> tokenlist ; // save token ( build tree ) 
} ; 

struct Tree {
  string content ;
  Tree *left ;
  Tree *right ;
  Tree *point ; // �ΨӦLdefine��tree 
  bool pro ; // �w�]��true �ؾ𪺮ɭԹJ��atom�|�ܦ�false ��evaluate�~�|���s�ܦ�true �D�n�B�zprocedure���D 
} ;

vector<Tree> gDef_name ; // �s��tree���c������ ���г����V�Qdefine�L���@�ʾ� ( �Φbwhich�o��function��define ) 
void GetToken( Token &token ) ;
vector<Tree*> gLocal_name ;
Tree * gErrorTree ; 

class Exception {
  public:
  string mMsg ;
  Exception() {  }

  Exception( string x ) {
    mMsg = x ;
  } // Exception()
    
} ; // class Expception

class Exception1 : public Exception {
  public:
  Exception1( string x ) {
    mMsg = x ;
  } // Exception1()
} ; // class Exception1

class Exception2 : public Exception {
  public:
  Exception2( string x ) {
    mMsg = x ;
  } // Exception2()
} ; // class Exception2

class Exception3 : public Exception {
  public:
  Exception3( string x ) {
    mMsg = x ;
  } // Exception3()
} ; // class Exception3 

class Exception4 : public Exception {
  public:
  Exception4() {
  } // Exception4()
} ; // class Exception4

class Exception5 : public Exception {
  public:
  Exception5() {
  } // Exception5()
} ; // class Exception5

class Exception6 : public Exception { // unbounded 
  public:
  Exception6( string x ) {
    mMsg = x ;
  } // Exception6()
} ; // class Exception6

class Exception7 : public Exception { // error argument
  public:
  Exception7( string x ) {
    mMsg = x ;
  } // Exception7()
} ; // class Exception7

class Exception8 : public Exception { // error argument
  public:
  Exception8( string x ) {
    mMsg = x ;
  } // Exception8()
} ; // class Exception8

class Exception9 : public Exception { // error argument
  public:
  Exception9() {
  } // Exception9()
} ; // class Exception9

class Exception10 : public Exception { // error argument
  public:
  Exception10() {
  } // Exception10()
} ; // class Exception10

char Getchar() {
  
  char ch = '\0' ;
  ch = getchar() ;
  
  if ( ch == '\n' ) {
    gcol = 1 ;
    gline++ ;
  } // if
  
  else {
    gcol++ ;
  } // else
  
  return ch ;
} // Getchar()

bool SkipWhiteSpace( char &ch_peek ) {
  char temp = '\0' ;
  while ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
    if ( ch_peek == '\n' )
      gWhite = true ;
    temp = Getchar() ;
    ch_peek = cin.peek() ;
  } // while
  
  return true ;
} // SkipWhiteSpace()

void PeekToken( Token &token ) {
  
  if ( ! g_bpeek ) {
    GetToken( token ) ;
    g_speek = token.curstr ; // (
    g_bpeek = true ;
  } // if
  
  else
    token.curstr = g_speek ; // (
  
} // PeekToken()

void GetToken( Token &token ) { //
  
  string str = "" ;
  char ch = '\0' ;
  char ch_peek = '\0' ;
  string temp = "" ; // �Ȯɩ���n��token 
  bool done = false ;
  
  if ( g_bpeek ) {
    PeekToken( token ) ;
    g_bpeek = false ;
    return ;
  } // if  
  
  else {
    ch_peek = cin.peek() ; // (
    SkipWhiteSpace( ch_peek ) ;
    gline_before = gline ;
    gcol_before = gcol ;

    while ( ! done ) {
  
      if ( ch_peek >= '0' && ch_peek <= '9' ) {
        ch = Getchar() ;
        bool bnum = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
          
        while ( ! bnum ) { // true�N��Ʀr�}�Ytoken���� 
            
          if ( ch_peek == '.' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // if
            
          else if ( ch_peek >= '0' && ch_peek <= '9' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else if
            
          else if ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
            bnum = true ;
            done = true ;
          } // else if 
            
          else if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '\'' || 
                    ch_peek == '"' || ch_peek == ';' ) {
            bnum = true ;
            done = true ;
          } // else if SYMBOL������ SYMBOL���]�t�o�� 
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
        } // while
      } // if
        
      else if ( ( ch_peek >= 'a' && ch_peek <= 'z' ) || ( ch_peek >= 'A' && ch_peek <= 'Z' ) ) {
        ch = Getchar() ;
        bool bvar = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
          
        while ( ! bvar ) {
            
          if ( ch_peek >= '0' && ch_peek <= '9' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // if
            
          else if ( ( ch_peek >= 'a' && ch_peek <= 'z' ) || ( ch_peek >= 'A' && ch_peek <= 'Z' ) ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else if
            
          else if ( ch_peek == '_' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else if
            
          else if ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
            bvar = true ;
            done = true ;
          } // else if
            
          else if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '\'' || 
                    ch_peek == '"' || ch_peek == ';' ) {
            bvar = true ;
            done = true ;
          } // else if SYMBOL������ SYMBOL���]�t�o��  
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
            
        } // while
      } // else if
        
      else if ( ch_peek == ')' ) { // �쪩���o�T�� 
        ch = Getchar() ;
        temp += ch ;
        done = true ;
      } // else if
        
      else if ( ch_peek == '(' ) { // (  ) ��case �n�⥦�X�֦b�@�_ 
        ch = Getchar() ;
        temp += ch ;
        ch_peek = cin.peek() ;
        SkipWhiteSpace( ch_peek ) ;
        if ( ch_peek == ')' ) {
          ch = Getchar() ;
          temp += ch ;
        } // if
          
        done = true ;
      } // else if
        
      else if ( ch_peek == '.' ) {
        ch = Getchar() ;
        bool bdot = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
          
        while ( ! bdot ) {
            
          if ( ch_peek >= '0' && ch_peek <= '9' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // if
            
          else if ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
            bdot = true ;
            done = true ;
          } // else if 
            
          else if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '\'' || 
                    ch_peek == '"' || ch_peek == ';' ) {
            bdot = true ;
            done = true ;
          } // else if SYMBOL������ SYMBOL���]�t�o��  
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
            
        } // while
      } // else if
        
      else if ( ch_peek == '_' ) { // �������u��case�|���Ҽ{ 
        ch = Getchar() ;
        bool bunderline = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
          
        while ( ! bunderline ) {
            
          if ( ( ch_peek >= 'a' && ch_peek <= 'z' ) || ( ch_peek >= 'A' && ch_peek <= 'Z' ) ||
               ch_peek == '_' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // if
            
          else if ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
            bunderline = true ;
            done = true ;
          } // else if
            
          else if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '\'' || 
                    ch_peek == '"' || ch_peek == ';' ) {
            bunderline = true ;
            done = true ;
          } // else if SYMBOL������ SYMBOL���]�t�o��  
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
        } // while
      } // else if 
        
      else if ( ch_peek == '#' ) { // //// #ffffff case 
        ch = Getchar() ;
        bool bany = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
        while ( ! bany ) {
            
          if ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
            bany = true ;
            done = true ;
          } // if
            
          else if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '\'' || 
                    ch_peek == '"' || ch_peek == ';' ) {
            bany = true ;
            done = true ;
          } // else if SYMBOL������ SYMBOL���]�t�o��  
          
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
            
        } // while
          
      } // else if
        
      else if ( ch_peek == '\'' ) {
        ch = Getchar() ;
        temp += ch ;
        done = true ;
      } // else if
        
      else if ( ch_peek == '"' ) { // �ĤT�ؿ��~ �S���ĤG�����޸� 
        ch = Getchar() ;
        bool bquote = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
        while ( ! bquote ) {
          if ( ch_peek != '\n' ) { // ���O���� 
            if ( ch_peek == '"' ) { 
              if ( temp[temp.size()-1] == '\\' ) {
                ch = Getchar() ;
                temp += ch ;
                ch_peek = cin.peek() ;
              } // if
                
              else {
                ch = Getchar() ;
                temp += ch ;
                done = true ;
                bquote = true ;
              } // else
            } // if
              
            else {
              ch = Getchar() ;
              temp += ch ;
              ch_peek = cin.peek() ;
            } // else
          } // if
            
          else { // �S�ݨ�ĤG�����޸� 
            gcol_before = gcol ;
            gline_before = gline ;
            throw new Exception3( temp ) ;
          } // else 
        } // while
      } // else if
        
      else if ( ch_peek == ';' ) { // line comments
        string str = "" ;
        char ch[100] ;
        char temp = '\0' ;
        cin.getline( ch, 100 ) ;
        if ( ! gSexpDone ) { // �o��Y�]��sexp�N����+1
          gline++ ;
          gline_before++ ;
        } // if
          
        gcol = 1 ; // ���� col = 1 
        gcol_before = 1 ;
        ch_peek = cin.peek() ;
        SkipWhiteSpace( ch_peek ) ; // �|�Y��white space �ҥH�����L 
        gline_before = gline ; // ////
        gcol_before = gcol ; // ////
        ch_peek = cin.peek() ;
        gSexpDone = false ;
      } // else if
        
      else if ( ch_peek == '+' || ch_peek == '-' ) {
        ch = Getchar() ;
        bool bsign = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
        while ( ! bsign ) {
            
          if ( ch_peek >= '0' && ch_peek <= '9' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // if
            
          else if ( ch_peek == '.' ) {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else if
            
          else if ( ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t' ) {
            bsign = true ;
            done = true ;
          } // else if
            
          else if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '\'' || 
                    ch_peek == '"' || ch_peek == ';' ) {
            bsign = true ;
            done = true ;
          } // else if SYMBOL������ SYMBOL���]�t�o��  
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else symbol
        } // while
      } // else if 
        
      else if ( ch_peek == EOF ) {
        throw new Exception4() ;
      } // else if
        
      else { // �@�ǩ_�Ǫ�symbol 
        ch = Getchar() ;
        temp += ch ;
        ch_peek = cin.peek() ;
        bool others = false ;
        while ( ! others ) {
          if ( ch_peek == '(' || ch_peek == ')' || ch_peek == '"' || ch_peek == '\'' ||
               ch_peek == ';' || ch_peek == ' ' || ch_peek == '\t' || ch_peek == '\n' ) {
            others = true ;
            done = true ;
          } // if
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
        } // while
      } // else
    } // while
  } // else
  
  if ( temp == "t" || temp == "#t" )
    temp = "#t" ;
  
  else if ( temp == "nil" || temp == "#f" || temp == "()" )
    temp = "nil" ;
  token.curstr = temp ;
} // GetToken()

bool IsString( string str ) {
  
  int index = 0 ;
  if ( str[index] == '"' ) { // �Ĥ@�ӬO���޸� 
    
    index++ ;
    while ( str[index] != '"' ) {
      
      if ( index != ( str.size() - 1 ) ) // 
        index++ ;
      else // size��index�@�ˤj�o�٨S��� " 
        return false ;
      
    } // while
    
    return true ;
  } // if
  
  else
    return false ;
  
} // IsString()

bool IsFloat( string str ) {
  
  int index = 0 ;
  bool not_float = false ;
  bool sign_dot = false ;
  
  if ( str[index] == '+' || str[index] == '-' ) {
    index++ ;
    sign_dot = true ;
  } // if
  
  if ( str[index] == '.' ) { // �Ĥ@��char�O�p���I .34 
  
    if ( str.size() != 1 ) { // ���O�u���@���I (�O�I) 
      index++ ;
      
      if ( index >= str.size() ) // ���� -. 
        return false ;
    } // if
    
    else
      return false ;
    
    while ( index < str.size() && ( str[index] >= '0' || str[index] <= '9' ) ) {

      if ( str[index] < '0' || str[index] > '9' ) {
        not_float = true ;
      } // if
      
      index++ ;
    } // while
  } // if
  
  else { // �Ĥ@��char���O�p���I eg 12.34 
    bool isdot = false ;
    
    while ( str[index] >= '0' && str[index] <= '9' ) {
      index++ ;
    } // while
    
    if ( str[index] == '.' ) {
      
      if ( ( str.size() - 1 ) == index ) // eg 123.
        return true ;
      
      index++ ;
      while ( str[index] >= '0' && str[index] <= '9' ) { // eg 12.34
        index++ ;
      } // while
      
      if ( str.size() == index )
        return true ;
      else // 12.3a
        return false ;
      
    } // if
    
    else // �P�ɧP�_�Ĥ@��char�O���O�Ʀrand �Ʀr�L��O���O�p���I 
      return false ;
    
  } // else
  
  if ( not_float )
    return false ;
  else
    return true ;
  
} // IsFloat()

bool IsInt( string str ) {
  
  int index = 0 ;
  bool not_int = false ;

  if ( str == "+" || str == "-" ) {
    return false ;
  } // if

  if ( str[index] == '+' || str[index] == '-' ) {
    index++ ;
  } // if
  
  while ( index < str.size() ) {
  
    if ( str[index] < '0' || str[index] > '9' ) {
      not_int = true ;
    } // if

    index++ ;
  } // while
  
  if ( not_int )
    return false ;
  else 
    return true ;
  
} // IsInt()

bool Atom( string str ) {
  
  if ( IsInt( str ) ) {
    return true ;
  } // if
  
  else if ( IsFloat( str ) ) {
    return true ;
  } // else if
  
  else if ( IsString( str ) ) {
    return true ;
  } // else if
  
  else if ( str == "nil" ) {
    return true ;
  } // else if
  
  else if ( str == "#t" ) {
    return true ;
  } // else if
  
  else if ( str == "()" ) {
    return true ;
  } // else if
  
  else if ( str == "(" || str == ")" || str == "." || str == "'" ) {
    return false ;
  } // else if
  
  else {
    return true ;
  } // else
  
  return false ;
} // Atom()

void Sexp( Token &token ) {

  PeekToken( token ) ; // ( 1
  if ( Atom( token.curstr ) ) {
    GetToken( token ) ;
    token.tokenlist.push_back( token.curstr ) ; // �S���Npush�i�h 
  } // if
  
  else if ( token.curstr == "(" ) {
    GetToken( token ) ;
    token.tokenlist.push_back( token.curstr ) ; // �S���Npush�i�h 
    Sexp( token ) ;

    PeekToken( token ) ; // 2
    while ( Atom( token.curstr ) || token.curstr == "(" || token.curstr == "'" ) {
      Sexp( token ) ;
      PeekToken( token ) ; // 2
    } // while

    if ( token.curstr == "." ) {
      GetToken( token ) ;
      token.tokenlist.push_back( token.curstr ) ; // �S���Npush�i�h 
      Sexp( token ) ;
    } // if
    
    PeekToken( token ) ;
    if ( token.curstr == ")" ) {
      GetToken( token ) ;
      token.tokenlist.push_back( token.curstr ) ; // �S���Npush�i�h 
    } // if
    
    else { //
      throw new Exception2( token.curstr ) ;
    } // else
    
  } // else if
  
  else if ( token.curstr == "'" ) {
    GetToken( token ) ;
    token.tokenlist.push_back( "(" ) ;
    token.tokenlist.push_back( "quote" ) ; // �S���Npush�i�h 
    Sexp( token ) ;
    token.tokenlist.push_back( ")" ) ;
  } // else if
  
  else {
    throw new Exception1( token.curstr ) ; 
  } // else
} // Sexp()

Tree * Build2( Token &token, int &i, bool new_node ) {

  Tree * node = new Tree() ; // ��node 
  node->pro = true ;
  node->content = "" ;
  if ( token.tokenlist[i] == "(" ) {
    i++ ;
      
    if ( new_node ) { //
      node->left = Build2( token, i, true ) ; // 1 2 
    } // if
      
    else { //
      node->left = Build2( token, i, false ) ; // 1 2 
    } // else
      
    i++ ;
    if ( token.tokenlist[i] == "(" || Atom( token.tokenlist[i] ) )
      new_node = true ; 
    else
      new_node = false ;
    node->right = Build2( token, i, new_node ) ; // 
    return node ;
  } // if
    
  else if ( token.tokenlist[i] == "." ) {
    i++ ;
    node = Build2( token, i, false ) ; 
    i++ ;
    return node ;
  } // else if
    
  else if ( token.tokenlist[i] == ")" ) {
    node->content = "nil" ;
    node->left = NULL ;
    node->right = NULL ;
    return node ;
  } // else if
    
  else if ( Atom( token.tokenlist[i] ) || token.tokenlist[i] == "quote" ) {
    if ( new_node ) {
      node->content = "" ;
      node->left = Build2( token, i, false ) ; 
      i++ ;
      if ( token.tokenlist[i] == "(" || Atom( token.tokenlist[i] ) )
        new_node = true ;
      else
        new_node = false ;    
      node->right = Build2( token, i, new_node ) ;
    } // if
      
    else {
      node->content = token.tokenlist[i] ;
      node->pro = false ; // //////
      node->left = NULL ;
      node->right = NULL ;
    } // else
  
    return node ;
  } // else if

  return 0 ; 
} // Build2()

bool IsFunction( string str ) {
  if ( str == "cons" || str == "list" || str == "car" || str == "cdr" || str == "atom?" || str == "+" ||
       str == "-" || str == "*" || str == "/" || str == "pair?" || str == "list?" || str == "null?" ||
       str == "integer?" || str == "real?" || str == "number?" || str == "string?" || str == "boolean?" ||
       str == "symbol?" || str == "not" || str == ">" || str == ">=" || str == "<" || str == "<=" || 
       str == "=" || str == "string-append" || str == "string>?" || str == "string<?" ||
       str == "string=?" || str == "eqv?" || str == "equal?" || str == "define" || str == "quote" ||
       str == "clean-environment" || str == "if" || str == "cond" || str == "begin" || str == "or" ||
       str == "and" || str == "exit" || str == "lambda" ) { // exit ?
    return true ;
  } // if
  
  return false ;
} // IsFunction()

bool ProFunction( Tree * ans ) {
  
  if ( ans->content == "cons" || ans->content == "list" || ans->content == "car" ||
       ans->content == "cdr" || ans->content == "atom?" || ans->content == "+" ||
       ans->content == "-" || ans->content == "*" || ans->content == "/" ||
       ans->content == "pair?" || ans->content == "list?" || ans->content == "null?" ||
       ans->content == "integer?" || ans->content == "real?" || ans->content == "number?" ||
       ans->content == "string?" || ans->content == "boolean?" || ans->content == "symbol?" ||
       ans->content == "not" || ans->content == ">" || ans->content == ">=" ||
       ans->content == "<" || ans->content == "<=" || ans->content == "=" ||
       ans->content == "string-append" || ans->content == "string>?" || ans->content == "string<?" ||
       ans->content == "string=?" || ans->content == "eqv?" || ans->content == "equal?" ||
       ans->content == "lambda" || ans->content == "exit" ) {
    return true ;
  } // if
  
  return false ;
} // ProFunction()

bool ProFunction_str( string str ) { // �t�O�u�O�ѼƬOstring 
  
  if ( str == "cons" || str == "list" || str == "car" || str == "cdr" || str == "atom?" || str == "+" ||
       str == "-" || str == "*" || str == "/" || str == "pair?" || str == "list?" || str == "null?" ||
       str == "integer?" || str == "real?" || str == "number?" || str == "string?" || str == "boolean?" ||
       str == "symbol?" || str == "not" || str == ">" || str == ">=" || str == "<" || str == "<=" || 
       str == "=" || str == "string-append" || str == "string>?" || str == "string<?" ||
       str == "string=?" || str == "eqv?" || str == "equal?" || str == "lambda" || str == "exit" ) {
    return true ;
  } // if
  
  return false ;
} // ProFunction_str()

void PrintType( Tree * ans ) {
  int i = 0 ; 
  
  if ( ProFunction( ans ) && ans->pro && ! gProPrint ) { // ��@�@��pro funcion 
    cout << "#<procedure " << ans->content << ">" ;
    return ;
  } // if
  
  if ( IsInt( ans->content ) ) {
    int itemp = atoi( ans->content.c_str() ) ;
    printf( "%d", itemp ) ;
  } // if
  
  else if ( IsFloat( ans->content ) ) {
    float ftemp = atof( ans->content.c_str() ) ;
    printf( "%.3f", ftemp ) ;
  } // else if
  
  else if ( IsString( ans->content ) ) {
    while ( i < ans->content.size() ) {
      bool isPrint = true ;
      if ( ans->content[i] == '\\' ) {
        if ( i + 1 < ans->content.size() && ans->content[i+1] == 'n' ) { // �{�b�O \ �U�@�ӬOn��
          i += 2 ;
          cout << endl ;
          isPrint = false ;
        } // if
        
        else if ( i + 1 < ans->content.size() && ans->content[i+1] == 't' ) {
          i += 2 ;
          cout << "    " ;
          isPrint = false ;
        } // else if
        
        else if ( i + 1 < ans->content.size() && ans->content[i+1] == '"' ) {
          i++ ;
        } // else if
        
        else if ( i + 1 < ans->content.size() && ans->content[i+1] == '\\' ) {
          i++ ;
        } // else if
      } // if
      
      if ( isPrint == true ) {
        cout << ans->content[i] ;
        i++ ;
      } // if
    } // while
  } // else if 
  
  else { // �ѤU��symbol 
    cout << ans->content ;
  } // else
} // PrintType()
  
void Print( Tree * ans, int &space ) {
  bool isleft = true ;
  if ( ans->content != "" && ans->content != "nil" ) { // if encounter atom then print 
    PrintType( ans ) ;
    cout << endl ;
  } // if
  
  else if ( ans->content == "nil" && gonly_one ) { // ���J��1�� and ��nil�� 
    cout << ans->content << endl ;
  } // else if
  
  else {
    if ( ans->left->content == "" ) {
      cout << "( " ;
      space++ ;
    } // if
    
    else if ( ans->left->content == "nil" ) { // ans->left->content == "nil"
      isleft = false ;
      cout << "nil" << endl ; 
    } // else if
    
    if ( isleft )
      Print( ans->left, space ) ;
    
    if ( ans->right->content == "nil" ) { // /// ans->right->content == nil
      space-- ;
    } // if
    
    if ( ans->content == "" ) {
      for ( int k = space ; k > 0 ; k-- ) {
        cout << "  " ;
      } // for
    } // if
    
    if ( ans->right->content == "nil" ) {
      cout << ")" << endl ;
    } // if
    
    else if ( ans->right != NULL && ans->right->content != "" && ans->right->content != "nil" ) { 
      cout << "." << endl ;
      for ( int k = space ; k > 0 ; k-- ) {
        cout << "  " ;
      } // for
      
      Print( ans->right, space ) ;
      space-- ;
      for ( int k = space ; k > 0 ; k-- ) {
        cout << "  " ;
      } // for
      
      cout << ")" << endl ;
    } // else if
    
    else if ( ans->right != NULL && ans->right->content == "" ) {
      Print( ans->right, space ) ;
    } // else if
  } // else
} // Print()

void IsExit( Tree * ans ) {
  
  if ( ans->content == "" ) {
    if ( ans->left->content == "exit" ) {
      if ( ans->left->left == NULL && ans->left->right == NULL ) {
        if ( ans->right->content == "nil" ) {
          if ( ans->right->left == NULL && ans->right->right == NULL ) {
            throw new Exception5() ;
          } // if
        } // if
      } // if
    } // if
  } // if
  
} // IsExit()

bool IsTreeEqual( Tree * tree1, Tree * tree2 ) {

  if ( tree1 == NULL && tree2 == NULL )
    return true ; 
  
  if ( ( tree1->content != tree2->content ) || ( tree1 != NULL && tree2 == NULL ) || 
       ( tree1 == NULL && tree2 != NULL ) ) {
    return false ;
  } // if
  
  else
    return IsTreeEqual( tree1->left, tree2->left ) && IsTreeEqual( tree1->right, tree2->right ) ;
} // IsTreeEqual()

int ErrorNum( string &str, Tree * temp ) {
  bool isTree = false ;
  if ( temp != NULL && temp->left != NULL && temp->right != NULL )
    isTree = true ;
  if ( IsFunction( str ) ) // �ݨ�o��function ���L 
    return -1 ;

  if ( ! gattempt && ( IsInt( str ) || IsFloat( str ) || IsString( str ) ) ) { // �o�����ӭn��Xtree 
    gErrorStr = str ;
    return 2 ;
  } // if
  
  if ( gattempt && isTree ) { // non func�Otree 
    cout << "ERROR (attempt to apply non-function) : " ;
    int space = 1 ;
    cout << "( " ;
    Print( temp, space ) ;
    return 2 ;
  } // if

  if ( str == "" || IsInt( str ) || IsFloat( str ) || IsString( str ) || str == "nil" || 
       str == "#t" ) {
    return -1 ; // �o�Ǥ���error 
  } // if
  
  int vector_has = 0 ;
  bool isDef = false ;
  int j = 0 ;
  for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �����ܼƦ��Q�w�q�L �B �O����function 
    if ( str == gDef_name[i].content ) {
      if ( ProFunction( gDef_name[i].point ) ) {
        vector_has = 1 ;
      } // if  
      
      else {
        vector_has = 2 ;
      } // else
      
      j = i ;
      isDef = true ;
    } // if
  } // for
  
  if ( vector_has == 0 ) { // ����function�O�H�K���� 
    gErrorStr = str ;
    return 1 ; // eg ( conss 3 5 )....
  } // if
  
  else if ( vector_has == 2 ) { // non func ���Otree 
    if ( isDef )
      gErrorStr = gDef_name[j].point->content ;
    else
      gErrorStr = str ;
    
    return 2 ;
  } // else if ( define a 5 ) , ( a 5 5 ) �n�L����node��content 
  
  return 0 ;
} // ErrorNum()

void ErrorMessage( int errorNum ) {
  
  if ( errorNum == -1 ) {
    return ;
  } // if
  
  else if ( errorNum == 1 ) {
    throw new Exception6( gErrorStr ) ; // unbounded
  } // else if
  
  else if ( errorNum == 2 ) {
    throw new Exception8( gErrorStr ) ; // attempt
  } // else if 
  
  else if ( errorNum == 3 ) {
    cout << "ERROR (DEFINE format) : ( " ;
    int space = 1 ; 
    Print( gErrorTree, space ) ;
    throw new Exception10() ;
  } // else if
  
  else if ( errorNum == 4 ) {
    cout << "ERROR (level of DEFINE)" << endl ;
    throw new Exception10() ;
  } // else if 
  
  else if ( errorNum == 5 ) {
    cout << "ERROR (non-list) : ( " ;
    int space = 1 ; 
    Print( gErrorTree, space ) ;
    throw new Exception10() ;
  } // else if 
  
  else if ( errorNum == 6 ) {
    
    if ( gErrorTree->content == "" ) { // �O�@�ʾ� 
      cout << "ERROR (" << gErrorStr << " with incorrect argument type) : ( " ;
      int space = 1 ;
      Print( gErrorTree, space ) ;
    } // if
  
    else {
      cout << "ERROR (" << gErrorStr << " with incorrect argument type) : " ;
      if ( IsFloat( gErrorTree->content ) ) {
        float ftemp = atof( gErrorTree->content.c_str() ) ;
        printf( "%.3f", ftemp ) ;
        cout << endl ;
      } // if

      else
        cout << gErrorTree->content << endl ;
    } // else
    
    throw new Exception10() ;
  } // else if
  
  else if ( errorNum == 7 ) {
    cout << "ERROR (division by zero) : /" << endl ;
    throw new Exception10() ;
  } // else if
  
  else if ( errorNum == 8 ) {
    gProPrint = true ;
    cout << "ERROR (no return value) : ( " ;
    int space = 1 ; 
    Print( gErrorTree, space ) ;
    throw new Exception10() ;
  } // else if
  
  else if ( errorNum == 9 ) { // cond error
    cout << "ERROR (COND format) : ( " ;
    int space = 1 ;
    Print( gErrorTree, space ) ;
    throw new Exception10() ;
  } // else if
  
  else if ( errorNum == 10 ) {
    gProPrint = true ;
    cout << "ERROR (no return value) : ( " ;
    int space = 1 ;
    Print( gErrorTree, space ) ;
    throw new Exception10() ;
  } // else if

  else if ( errorNum == 11 ) {
    cout << "ERROR (level of EXIT)" << endl ;
    throw new Exception10() ;
  } // else if
  
  else if ( errorNum == 12 ) {
    
    if ( gErrorTree->content == "" ) { // �O�@�ʾ� 
      cout << "ERROR (attempt to apply non-function) : ( " ;
      int space = 1 ;
      Print( gErrorTree, space ) ;
    } // if
  
    else {
      cout << "ERROR (attempt to apply non-function) : " ;
      cout << gErrorTree->content << endl ;
    } // else
    
    throw new Exception10() ;
  } // else if
  
} // ErrorMessage()

int Num_argument( Tree * temp ) {
  int arg = -1 ;
  while ( temp->right != NULL ) {
    arg++ ;
    temp = temp->right ;
  } // while
  
  return arg ;
} // Num_argument()

Tree * Lambda( Tree * temp, string str ) {
  
  
  return 0 ;
} // Lambda()

Tree * Which( Tree * ans ) {

  // if ( gfirst_which == 0 && ans->left->content == "lambda" ) {
  //  gproLambda = true ;
  // } // if

  Tree * nonlist = new Tree() ;
  nonlist = ans->right ;
  while ( nonlist != NULL ) { // nonlist error 
    if ( nonlist->content != "" && nonlist->content != "nil" && ! gnon ) {
      gErrorTree = ans ;
      ErrorMessage( 5 ) ;
    } // if
  
    nonlist = nonlist->right ;
  } // while
  
  gfirst_which++ ;
  Tree * proWalk = new Tree() ;
  bool isPro = false ;
  proWalk = ans ;
  if ( proWalk->right != NULL && proWalk->right->content != "" ) {
    if ( IsFunction( proWalk->left->content ) ) // 2-8 new
      isPro = false ;
    else
      isPro = true ;
  } // if
  
  for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �����ܼƦ��Q�w�q�L �B �O����function 
    if ( ans->content == gDef_name[i].content ) {
      if ( ProFunction( gDef_name[i].point ) ) {
        ans->content = gDef_name[i].point->content ;
      } // if
    } // if
  } // for
  
  if ( ans->left != NULL && ( ans->left->content == "quote" ) ) {
    if ( Num_argument( ans ) != 1 ) { // �n�X�ӰѼ� 
      throw new Exception7( "quote" ) ;
    } // if
  
    gattempt = true ; 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    temp->content = "" ;
    temp = ans->right->left ;
    return temp ;
  } // if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "cons" || Which( ans->left )->content == "cons" ) && ! isPro ) {
    gattempt = true ; // ���i�ӥN��function���W�r�O���T�� 
    if ( Num_argument( ans ) != 2 ) {
      throw new Exception7( "cons" ) ;
    } // if
  
    Tree * temp = new Tree() ;
    temp->pro = true ;
    temp->content = "" ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gnon = true ; // �J���ܼƪ��ɭ� ��nonlist�|�� �ҥH�h�[�@�ӱ��� 
        temp->left = Which( gDef_name[i].point ) ;
        gnon = false ; // �Χ����� 
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
        gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
      } // if
        
      temp->left = Which( ans->right->left ) ;
    } // if
    
    isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->right->left->content == gDef_name[i].content ) {
        gnon = true ;
        temp->right = Which( gDef_name[i].point ) ;
        gnon = false ;
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
  
    if ( ! isDef_exist ) { 
      if ( ans->right->right->left->left != NULL && ans->right->right->left->right != NULL )
        gattempt = false ;

      temp->right = Which( ans->right->right->left ) ;
    } // if 
    
    return temp ;
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "list" || Which( ans->left )->content == "list" ) && ! isPro ) { 
    if ( Num_argument( ans ) < 0 ) { // �n�X�ӰѼ� 
      throw new Exception7( "list" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // �nreturn������ 
    Tree * temp = new Tree() ;
    temp->left = NULL ;
    temp->right = NULL ;
    re_node->left = NULL ;
    re_node->right = NULL ;
    temp->pro = true ;
    re_node->pro = true ;
    re_node = temp ; // ������̤@�}�l 
    temp->content = "" ;
    if ( ans->right != NULL ) {
      ans = ans->right ;  
    } // if
    
    if ( ans->content == "nil" ) { // ( list ) �|��Xnil�����p 
      temp->content = "nil" ;
      return temp ;
    } // if
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->left->content == gDef_name[i].content ) {
        gnon = true ;
        temp->left = Which( gDef_name[i].point ) ;
        gnon = false ;
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist ) { // �ݦ��L�Q�w�q�L
      int num = ErrorNum( ans->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) {
      if ( ans->left->left != NULL && ans->left->right != NULL )
        gattempt = false ;
      temp->left = Which( ans->left ) ;
    } // if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      Tree * tmp = new Tree() ;
      tmp->left = NULL ;
      tmp->right = NULL ;
      tmp->pro = true ;
      temp->right = tmp ;
      tmp->content = "" ;
      ans = ans->right ;
      
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          gnon = true ;
          tmp->left = Which( gDef_name[i].point ) ;
          gnon = false ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist ) { // �ݦ��L�Q�w�q�L 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist ) {
        if ( ans->left->left != NULL && ans->left->right != NULL )
          gattempt = false ;
        tmp->left = Which( ans->left ) ;
      } // if
      
      temp = temp->right ;
      isDef_exist = false ;
    } // while

    temp->right = Which( ans->right ) ; // �X�j��N��k��ѤUnil �A�@�����j�Y�i���Wnil 
    return re_node ;
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "car" || Which( ans->left )->content == "car" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "car" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    temp->content = "" ;
    temp->pro = true ;
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = gDef_name[i].point ; // ////////////////////////////// 2-6 new
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist )
      temp = Which( ans->right->left ) ;

    if ( temp->content != "" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "car" ; 
      ErrorMessage( 6 ) ;
    } // if

    return temp->left ;
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "cdr" || Which( ans->left )->content == "cdr" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "cdr" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    temp->content = "" ;
    temp->pro = true ;
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = gDef_name[i].point ; // 2-6 new
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist )
      temp = Which( ans->right->left ) ;
      
    if ( temp->content != "" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "cdr" ; 
      ErrorMessage( 6 ) ;
    } // if
      
    return temp->right ;
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "define" ) { // define again ( define a b )
  
    if ( gfirst_which > 5 ) {
      ErrorMessage( 4 ) ; // ( define a ( define b 5 ) .... 
    } // if
  
    if ( Num_argument( ans ) != 2 ) { // �n�X�ӰѼ� 
      gErrorTree = ans ;
      ErrorMessage( 3 ) ;
    } // if
  
    if ( IsFunction( ans->right->left->content ) || IsInt( ans->right->left->content ) ||
         IsFloat( ans->right->left->content ) || IsString( ans->right->left->content ) ||
         ans->right->left->content == "#t" || ans->right->left->content == "nil" ) {
      gErrorTree = ans ; // �n�Q�w�q���ܼƬO����func 
      ErrorMessage( 3 ) ; 
    } // if
  
    gattempt = true ;
    Tree temp ;
    temp.pro = true ;
    temp.content = ans->right->left->content ; // tree���W�r 
    bool reDef = false ; // �d�ߦ��ܼƬO�_�Q���sdefine
    bool isDef_exist = false ; 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �ݾ𪺦W�r���L�Q�w�q�L 
      if ( temp.content == gDef_name[i].content ) { // ���Q�w�q�L eg ( define "a" b ) 
        for ( int j = 0 ; j < gDef_name.size() ; j++ ) { // �A�ݫ᭱�O�_�O�w�w�q�L��tree  eg ( define a "b" ) 
          if ( ans->right->right->left->content == gDef_name[j].content ) { // ���᭱��tree 
            gDef_name[i].point = gDef_name[j].point ; // �e�����ܼƫ��V�᭱�� eg a point to b 
            isDef_exist = true ; 
          } // if
        } // for
        
        if ( ! isDef_exist )
          gDef_name[i].point = Which( ans->right->right->left ) ; // �Y���Q�w�q�L �h�ӫ��Э��s���V�s�� 

        reDef = true ; 
      } // if
    } // for
    
    if ( ! reDef ) { // �Y�S�Q�w�q�L �h�]�@�����j �M��point push�ivector
      for ( int j = 0 ; j < gDef_name.size() ; j++ ) {
        if ( ans->right->right->left->content == gDef_name[j].content ) {
          temp.point = gDef_name[j].point ;
          isDef_exist = true ; 
        } // if
      } // for
      
      if ( ! isDef_exist ) {
        temp.point = Which( ans->right->right->left ) ;
      } // if
        
      gDef_name.push_back( temp ) ;
    } // if
    
    cout << temp.content << " defined" << endl ;
    gIs_Define = true ; // �ΨӤ���print tree�Ϊ� 
    return 0 ;
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "clean-environment" ) {

    if ( gfirst_which > 5 ) { // �]���e�������j�n�] �ҥH�ܦ��n�j��5 
      throw new Exception9() ;
    } // if
  
    if ( Num_argument( ans ) != 0 ) {
      throw new Exception7( "clean-environment" ) ;
    } // if

    gattempt = true ; 
    if ( ans->right->content == "nil" && ans->left->right == NULL && ans->left->left == NULL ) {
      gDef_name.clear() ;
      cout << "environment cleaned" << endl ;
      gIs_Define = true ;
    } // if
  } // else if
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "exit" || Which( ans->left )->content == "exit" ) && ! isPro ) {
    if ( gfirst_which > 5 ) {
      ErrorMessage( 11 ) ;
    } // if

    if ( Num_argument( ans ) > 0 ) {
      throw new Exception7( "exit" ) ;
    } // if
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "atom?" || Which( ans->left )->content == "atom?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "atom?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
    
    if ( temp->left == NULL && temp->right == NULL ) {
      // IsInt( temp->content ) || IsFloat( temp->content ) || IsString( temp->content )
      n_temp->content = "#t" ;
    } // if �u���@��node�N�Oatom ���i��|�����D 
    
    else {
      n_temp->content = "nil" ;
    } // else
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "pair?" || Which( ans->left )->content == "pair?" ) && ! isPro ) {  
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "pair?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ; // ���@�ʾ�~�Otrue �Ϥ��N�Oatom���@��node
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
    
    if ( temp->left != NULL && temp->right != NULL ) {
      n_temp->content = "#t" ;
    } // if
    
    else {
      n_temp->content = "nil" ;
    } // else
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "list?" || Which( ans->left )->content == "list?" ) && ! isPro ) { 
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "list?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ; // �̥k��Onil�~�Otrue 
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
    
    while ( temp->right != NULL ) {
      temp = temp->right ;
    } // while
    
    if ( temp->content == "nil" ) {
      n_temp->content = "#t" ;
    } // if
    
    else {
      n_temp->content = "nil" ;
    } // else
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "null?" || Which( ans->left )->content == "null?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "null?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ; // �k�䪺����Onil 
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
      
    if ( temp->content == "nil" ) {
      n_temp->content = "#t" ;
    } // if
  
    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "integer?" || Which( ans->left )->content == "integer?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "integer?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ; // �k�䪺����Oint
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
      
    if ( IsInt( temp->content ) ) {
      n_temp->content = "#t" ;
    } // if
  
    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "real?" || ans->left->content == "number?" ||
              Which( ans->left )->content == "real?" || Which( ans->left )->content == "number?" ) &&
            ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      if ( ans->content == "real?" )
        throw new Exception7( "real?" ) ;
      else
        throw new Exception7( "number?" ) ;
    } // if

    Tree * temp = new Tree() ; // �k�䪺����O��Ʃ�number ( is-real IFF is-number ) 
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
      
    if ( IsInt( temp->content ) || IsFloat( temp->content ) ) {
      n_temp->content = "#t" ;
    } // if
  
    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "string?" || Which( ans->left )->content == "string?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "string?" ) ;
    } // if

    Tree * temp = new Tree() ; // �k�䪺����Ostring
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
      
    if ( IsString( temp->content ) ) {
      n_temp->content = "#t" ;
    } // if
  
    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "boolean?" || Which( ans->left )->content == "boolean?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "boolean?" ) ;
    } // if

    Tree * temp = new Tree() ; // �k�䪺����Obool
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
      
    if ( temp->content == "#t" || temp->content == "nil" ) {
      n_temp->content = "#t" ;
    } // if
  
    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "symbol?" || Which( ans->left )->content == "symbol?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "symbol?" ) ;
    } // if

    Tree * temp = new Tree() ; // �k�䪺����Osymbol
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �ݦ��L�Q�w�q�L 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
        gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
      } // if

      temp = Which( ans->right->left ) ;
    } // if
      
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) &&
           ! IsString( temp->content ) ) ) {
      n_temp->content = "#t" ;
    } // if

    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "+" || Which( ans->left )->content == "+" ) && ! isPro ) {
    if ( Num_argument( ans ) < 2 ) {
      throw new Exception7( "+" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // �nreturn������ 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 0 ;
    float f_res = 0.0 ;
    char *ch = new char[100] ; // �ΨӸ� sprintf���F�� 
    bool float_type = false ; // �Y�Otrue �h��X���ɭԭn��float���A 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �Ψӧ��ܼƸ̭����F�� 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // �� �h���_�� 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) {
      int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
      ErrorMessage( num ) ;
    } // if

    if ( ! isDef_exist ) // �u�O�@�몺�F�� 
      temp = Which( ans->left ) ;
    
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "+" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // �ݱ����F�誺���A�O���� 
      f_res += atoi( temp->content.c_str() ) ; // ������float���A�� 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // ��true���� �N���|�ܦ^false �]���̫᪽���n��float�L 
      f_res += atof( temp->content.c_str() ) ;
    } // else if
      
    else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
    } // else
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �@�ˬO���ܼƸ̭����F�� 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
    
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) {
        int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
        ErrorMessage( num ) ;
      } // if
    
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = tmp ;
        gErrorStr = "+" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // �P�W�� 
        f_res += atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        f_res += atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // �Otrue �h�����নfloat 
      sprintf( ch, "%f", f_res ) ;
    else { // �N��O�n�Lint���A 
      i_res = f_res ;
      sprintf( ch, "%d", i_res ) ;
    } // else
    
    re_node->content = ch ;
    return re_node ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "-" || Which( ans->left )->content == "-" ) && ! isPro ) { 
    if ( Num_argument( ans ) < 2 ) {
      throw new Exception7( "-" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // �nreturn������ 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 0 ;
    float f_res = 0.0 ;
    char *ch = new char[100] ; // �ΨӸ� sprintf���F�� 
    bool float_type = false ; // �Y�Otrue �h��X���ɭԭn��float���A 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �Ψӧ��ܼƸ̭����F�� 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // �� �h���_�� 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) { 
      int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // �u�O�@�몺�F�� 
      temp = Which( ans->left ) ;
  
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "-" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // �ݱ����F�誺���A�O���� 
      f_res += atoi( temp->content.c_str() ) ; // ������float���A�� 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // ��true���� �N���|�ܦ^false �]���̫᪽���n��float�L 
      f_res += atof( temp->content.c_str() ) ;
    } // else if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �@�ˬO���ܼƸ̭����F�� 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { 
        int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = tmp ;
        gErrorStr = "-" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // �P�W�� 
        f_res -= atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        f_res -= atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // �Otrue �h�����নfloat 
      sprintf( ch, "%f", f_res ) ;
    else { // �N��O�n�Lint���A 
      i_res = f_res ;
      sprintf( ch, "%d", i_res ) ;
    } // else
    
    re_node->content = ch ;
    return re_node ;
  } // else if 
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "*" || Which( ans->left )->content == "*" ) && ! isPro ) {
    if ( Num_argument( ans ) < 2 ) {
      throw new Exception7( "*" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // �nreturn������ 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 1 ;
    float f_res = 1.0 ;
    char *ch = new char[100] ; // �ΨӸ� sprintf���F�� 
    bool float_type = false ; // �Y�Otrue �h��X���ɭԭn��float���A 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �Ψӧ��ܼƸ̭����F�� 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // �� �h���_�� 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) {
      int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // �u�O�@�몺�F�� 
      temp = Which( ans->left ) ;
  
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "*" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // �ݱ����F�誺���A�O���� 
      f_res *= atoi( temp->content.c_str() ) ; // ������float���A�� 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // ��true���� �N���|�ܦ^false �]���̫᪽���n��float�L 
      f_res *= atof( temp->content.c_str() ) ;
    } // else if
      
    else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
    } // else
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �@�ˬO���ܼƸ̭����F�� 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) {
        int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = tmp ;
        gErrorStr = "*" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // �P�W�� 
        f_res *= atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        f_res *= atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // �Otrue �h�����নfloat 
      sprintf( ch, "%f", f_res ) ;
    else { // �N��O�n�Lint���A 
      i_res = f_res ;
      sprintf( ch, "%d", i_res ) ;
    } // else
    
    re_node->content = ch ;
    return re_node ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "/" || Which( ans->left )->content == "/" ) && ! isPro ) {
    if ( Num_argument( ans ) < 2 ) {
      throw new Exception7( "/" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // �nreturn������ 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 1 ;
    float f_res = 1.0 ;
    char *ch = new char[100] ; // �ΨӸ� sprintf���F�� 
    bool float_type = false ; // �Y�Otrue �h��X���ɭԭn��float���A 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �Ψӧ��ܼƸ̭����F�� 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // �� �h���_�� 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) {
      int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // �u�O�@�몺�F�� 
      temp = Which( ans->left ) ;
  
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "/" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // �ݱ����F�誺���A�O���� 
      int tmp = atoi( temp->content.c_str() ) ; // =========== can not divide zero ============
      f_res = tmp ; // ������float���A�� 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // ��true���� �N���|�ܦ^false �]���̫᪽���n��float�L 
      float tmp = atof( temp->content.c_str() ) ;
      f_res = tmp ;
    } // else if
      
    else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
    } // else
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �@�ˬO���ܼƸ̭����F�� 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) {
        int num = ErrorNum( ans->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = tmp ;
        gErrorStr = "/" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // �P�W�� 
      
        if ( atoi( tmp->content.c_str() ) == 0 )  // ���H0 
          ErrorMessage( 7 ) ; 
        f_res /= atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        if ( atof( tmp->content.c_str() ) == 0.0 )
          ErrorMessage( 7 ) ; 
        f_res /= atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // �o��|�X�{error �]�������ܼƸ˪����Oint or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // �Otrue �h�����নfloat 
      sprintf( ch, "%f", f_res ) ;
    else { // �N��O�n�Lint���A 
      i_res = f_res ;
      sprintf( ch, "%d", i_res ) ;
    } // else
    
    re_node->content = ch ;
    return re_node ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "not" || Which( ans->left )->content == "not" ) && ! isPro ) { 
    if ( Num_argument( ans ) != 1 ) {
      throw new Exception7( "not" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ; // ���ϦV���N�� 
    Tree * n_temp = new Tree() ; 
    temp->pro = true ;
    n_temp->pro = true ;
    temp->content = "" ;
    
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist ) {
      int num = ErrorNum( ans->right->left->content, NULL ) ; // �ݦ��L�Q�w�q�L 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    } // if
      
    if ( temp->content == "nil" ) {
      n_temp->content = "#t" ;
    } // if
  
    else 
      n_temp->content = "nil" ;
      
    return n_temp ;
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == ">" || ans->left->content == ">=" || ans->left->content == "<" ||
              ans->left->content == "<=" || ans->left->content == "=" || 
              Which( ans->left )->content == ">" || Which( ans->left )->content == ">=" ||
              Which( ans->left )->content == "<" || Which( ans->left )->content == "<=" ||
              Which( ans->left )->content == "=" ) && ! isPro ) {
    if ( Num_argument( ans ) < 2 ) {
      if ( ans->left->content == ">" )
        throw new Exception7( ">" ) ;
      if ( ans->left->content == "<" )
        throw new Exception7( "<" ) ;
      if ( ans->left->content == ">=" )
        throw new Exception7( ">=" ) ;
      if ( ans->left->content == "<=" )
        throw new Exception7( "<=" ) ;
      if ( ans->left->content == "=" )
        throw new Exception7( "=" ) ;
    } // if

    string str = ans->left->content ;
    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * n_temp = new Tree() ; // �̫�return��node 
    Tree * head = new Tree() ;
    temp->pro = true ;
    n_temp->pro = true ;
    head->pro = true ;
    head->content = "" ;
    head = ans ;
    temp->content = "" ;
    float f_res1 = 0.0 ;
    float f_res2 = 0.0 ;
    bool isFalse = false ;
    
    Tree * unbound = new Tree() ;
    unbound = ans ;
    bool isDef_exist = false ; // �d��cons�᭱��tree �O�_���w�w�q���ܼ� 
    while ( unbound->right->content != "nil" ) {
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( unbound->right->left->content == gDef_name[i].content ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ; // ��즳�ܼƳQ�w�q 
        } // if
      } // for
     
      if ( ! isDef_exist && ! IsInt( unbound->right->left->content ) && 
           ! IsFloat( unbound->right->left->content ) &&
           ! IsString( unbound->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
        int num = ErrorNum( unbound->right->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
    
      if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
        temp = Which( unbound->right->left ) ;
    
      if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
           ( temp->left != NULL && temp->right != NULL ) ||
           temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = temp ;
        gErrorStr = str ; // < > <= >= =
        ErrorMessage( 6 ) ;
      } // if
    
      unbound = unbound->right ;
    } // while
    
    isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;

    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = str ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if

    if ( ans->right != NULL )
      ans = ans->right ; 
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" && ! isFalse ) { // ����nil�N��w���� error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �@�ˬO���ܼƸ̭����F�� 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = tmp ;
        gErrorStr = str ; // < > <= >= =
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( temp->content ) || IsFloat( temp->content ) ) {
        f_res1 = atof( temp->content.c_str() ) ;
      } // if
    
      if ( IsInt( tmp->content ) || IsFloat( tmp->content ) ) {
        f_res2 = atof( tmp->content.c_str() ) ;
      } // if
    
      if ( head->left->content == ">" ) {
        if ( f_res1 > f_res2 )
          n_temp->content = "#t" ;
        else
          n_temp->content = "nil" ;
      } // if
  
      if ( head->left->content == ">=" ) {
        if ( f_res1 >= f_res2 )
          n_temp->content = "#t" ;
        else
          n_temp->content = "nil" ;
      } // if
  
      if ( head->left->content == "<" ) {
        if ( f_res1 < f_res2 )
          n_temp->content = "#t" ;
        else
          n_temp->content = "nil" ;
      } // if
  
      if ( head->left->content == "<=" ) {
        if ( f_res1 <= f_res2 )
          n_temp->content = "#t" ;
        else
          n_temp->content = "nil" ;
      } // if
  
      if ( head->left->content == "=" ) {
        if ( f_res1 == f_res2 )
          n_temp->content = "#t" ;
        else
          n_temp->content = "nil" ;
      } // if
      
      if ( n_temp->content == "nil" )
        isFalse = true ;
      
      temp = tmp ;
      isDef_exist = false ;
    } // while
  
    return n_temp ;
  } // else if
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "string-append" || Which( ans->left )->content == "string-append" ) &&
            ! isPro ) { // string +�b�@�_ error
    if ( Num_argument( ans ) < 2 ) {
      throw new Exception7( "string-append" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // �nreturn������ 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    temp->content = "" ;
    re_node->content = "" ; // "az" + "ax" = "azax" ���������޸��n�h�� 
    if ( ans->right != NULL ) {
      ans = ans->right ;
    } // if
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist )
      temp = Which( ans->left ) ;
    
    if ( ! IsString( temp->content ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = "string-append" ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    
    if ( IsString( temp->content ) ) {
      for ( int i = 1 ; i < temp->content.size() - 1 ; i++ )
        re_node->content += temp->content[i] ;
    } // if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        temp = Which( ans->left ) ;
        
      if ( ! IsString( temp->content ) ||
           ( temp->left != NULL && temp->right != NULL ) ||
           temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = temp ;
        gErrorStr = "string-append" ; // < > <= >= =
        ErrorMessage( 6 ) ;
      } // if
        
      if ( IsString( temp->content ) ) {
        for ( int i = 1 ; i < temp->content.size() - 1 ; i++ )
          re_node->content += temp->content[i] ;
      } // if
    
      else { // error
      } // else
        
      isDef_exist = false ;
    } // while

    re_node->content = "\"" + re_node->content + "\"" ;
    return re_node ;
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "string>?" || ans->left->content == "string<?" ||
              ans->left->content == "string=?" || Which( ans->left )->content == "string>?" ||
              Which( ans->left )->content == "string<?" || Which( ans->left )->content == "string=" ) &&
            ! isPro ) {
    if ( Num_argument( ans ) < 2 ) {
      if ( ans->left->content == "string>?" )
        throw new Exception7( "string>?" ) ;
      if ( ans->left->content == "string<?" )
        throw new Exception7( "string<?" ) ;
      if ( ans->left->content == "string=?" )
        throw new Exception7( "string=?" ) ;
    } // if
    
    string str = ans->left->content ;
    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * tmp = new Tree() ;
    Tree * n_temp = new Tree() ;
    Tree * re_temp = new Tree() ; // �̫�n�^�Ǫ�node
    temp->pro = true ;
    n_temp->pro = true ;
    tmp->pro = true ;
    re_temp->pro = true ; 
    n_temp = ans->left ; // ���O�d�@�}�l��ans���� ���Vleft���N��O�L��content�N�Ostring>?������ 
    temp->content = "" ;
    tmp->content = "" ;
    bool isFalse = false ; // �p�G���~���@�Ӥ�����~ ���L�N�ܦ�true �̫�return nil 
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 
    
    Tree * unbound = new Tree() ;
    unbound = ans ;
    
    while ( unbound->right->content != "nil" ) {
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( unbound->right->left->content == gDef_name[i].content ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ; // ��즳�ܼƳQ�w�q 
        } // if
      } // for
     
      if ( ! isDef_exist && ! IsInt( unbound->right->left->content ) &&
           ! IsFloat( unbound->right->left->content ) &&
           ! IsString( unbound->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
        int num = ErrorNum( unbound->right->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
    
      if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
        temp = Which( unbound->right->left ) ;
    
      if ( ! IsString( temp->content ) ||
           ( temp->left != NULL && temp->right != NULL ) ||
           temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = temp ;
        gErrorStr = str ; // < > <= >= =
        ErrorMessage( 6 ) ;
      } // if
    
      unbound = unbound->right ;
    } // while
    
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left ) ;
    
    if ( ! IsString( temp->content ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp ;
      gErrorStr = str ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    
    if ( ans->right != NULL ) {
      ans = ans->right ;  
    } // if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" && ! isFalse ) { // ����nil�N��w���� error
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ! IsString( tmp->content ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // ��ܥu���@��node 
        gErrorTree = tmp ;
        gErrorStr = str ; // < > <= >= =
        ErrorMessage( 6 ) ;
      } // if
    
      if ( n_temp->content == "string>?" ) {
        if ( temp->content > tmp->content )
          re_temp->content = "#t" ;
        else
          isFalse = true ;
      } // if
      
      else if ( n_temp->content == "string<?" ) {
        if ( temp->content < tmp->content )
          re_temp->content = "#t" ;
        else
          isFalse = true ;
      } // else if
      
      else if ( n_temp->content == "string=?" ) {
        if ( temp->content == tmp->content )
          re_temp->content = "#t" ;
        else
          isFalse = true ;
      } // else if
        
      temp = tmp ;
      isDef_exist = false ;
    } // while
    
    if ( isFalse )
      re_temp->content = "nil" ;
    else
      re_temp->content = "#t" ;
    
    return re_temp ;
  } // else if
  
  else if ( ans->left != NULL && 
            ( ans->left->content == "eqv?" || Which( ans->left )->content == "eqv?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 2 ) {
      throw new Exception7( "eqv?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp1 = new Tree() ;
    Tree * temp2 = new Tree() ;
    Tree * tmp = new Tree() ; 
    Tree * re_node = new Tree() ; // return
    temp1->pro = true ;
    re_node->pro = true ;
    temp2->pro = true ;
    tmp->pro = true ;
    temp2->content = "" ; // �ĤG��tree 
    temp1->content = "" ; // �Ĥ@��tree 
    tmp->content = "" ; 
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp1 = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    tmp = ans->right->left ; // �Ȯɫ��V�Ĥ@�ʾ� 
    if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
      temp1 = Which( ans->right->left ) ;
    
    if ( ! IsInt( temp1->content ) && ! IsFloat( temp1->content ) && 
         ! IsString( temp1->content ) && temp1->content != "#t" && temp1->content != "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp1 ;
      gErrorStr = "eqv?" ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    
    bool isDef_exist2 = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->right->left->content == gDef_name[i].content ) {
        temp2 = Which( gDef_name[i].point ) ;
        isDef_exist2 = true ;
      } // if
    } // for
    
    if ( ! isDef_exist2 && ! IsInt( ans->right->right->left->content ) && 
         ! IsFloat( ans->right->right->left->content ) &&
         ! IsString( ans->right->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    tmp = ans->right->right->left ; // �Ȯɫ��V�ĤG�ʾ� 
    if ( ! isDef_exist2 ) // �S��� ��������~�򻼰j 
      temp2 = Which( ans->right->right->left ) ;

    if ( ! IsInt( temp2->content ) && ! IsFloat( temp2->content ) &&
         ! IsString( temp2->content ) && temp2->content != "#t" && temp2->content != "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp2 ;
      gErrorStr = "eqv?" ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if

    if ( isDef_exist && isDef_exist2 && temp1 != temp2 ) {
      re_node->content = "nil" ;
      return re_node ;
    } // if
  
    else if ( isDef_exist && isDef_exist2 && temp1 == temp2 ) {
      re_node->content = "#t" ;
      return re_node ;
    } // else if
  
    if ( isDef_exist && isDef_exist2 ) { // ��ӳ��O�H�w�q�ܼ� �B ��ӦW�r���n�@��  (���@�˷|��Xnil 
      if ( ans->right->left->content == ans->right->right->left->content )
        re_node->content = "#t" ;
      else
        re_node->content = "nil" ;
    } // if
  
    else if ( ! isDef_exist && ! isDef_exist2 ) {

      if ( ( IsInt( temp1->content ) || IsFloat( temp1->content ) || temp1->content == "#t" ||
             temp1->content == "nil" ) && temp1->left == NULL && temp1->right == NULL ) {
        if ( ( ( IsInt( temp2->content ) || IsFloat( temp2->content ) || temp2->content == "#t" ||
                 temp2->content == "nil" ) && temp2->left == NULL && temp2->right == NULL ) ) {
          if ( temp1->content == temp2->content )
            re_node->content = "#t" ;
          else
            re_node->content = "nil" ;
        } // if  
      
        else
          re_node->content = "nil" ;
      } // if
    
      else
        re_node->content = "nil" ;
    } // else if
    
    else if ( ( isDef_exist && ! isDef_exist2 ) || ( ! isDef_exist && isDef_exist2  ) ) {
      if ( temp1->content == temp2->content && temp1->content != "" ) {
        re_node->content = "#t" ;
      } // if
      
      else
        re_node->content = "nil" ;
    } // else if
  
    else
      re_node->content = "nil" ;

    return re_node ;
  } // else if 
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "equal?" || Which( ans->left )->content == "equal?" ) && ! isPro ) {
    if ( Num_argument( ans ) != 2 ) {
      throw new Exception7( "equal?" ) ;
    } // if

    gattempt = true ; 
    Tree * temp1 = new Tree() ;
    Tree * temp2 = new Tree() ;
    Tree * re_node = new Tree() ; // return
    temp1->pro = true ;
    temp2->pro = true ;
    re_node->pro = true ;
    temp2->content = "" ; // �ĤG��tree 
    temp1->content = "" ; // �Ĥ@��tree 
    bool tf = false ;
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp1 = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
      temp1 = Which( ans->right->left ) ;
    /*
    if ( ! IsInt( temp1->content ) && ! IsFloat( temp1->content ) && 
         ! IsString( temp1->content ) && temp1->content != "#t" && temp1->content != "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp1 ;
      gErrorStr = "equal?" ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    */

    bool isDef_exist2 = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->right->left->content == gDef_name[i].content ) {
        temp2 = Which( gDef_name[i].point ) ;
        isDef_exist2 = true ;
      } // if
    } // for
    
    if ( ! isDef_exist2 && ! IsInt( ans->right->right->left->content ) && 
         ! IsFloat( ans->right->right->left->content ) &&
         ! IsString( ans->right->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist2 ) // �S��� ��������~�򻼰j 
      temp2 = Which( ans->right->right->left ) ;
    /*
    if ( ! IsInt( temp2->content ) && ! IsFloat( temp2->content ) && 
         ! IsString( temp2->content ) && temp2->content != "#t" && temp2->content != "nil" ) { // ��ܥu���@��node 
      gErrorTree = temp2 ;
      gErrorStr = "equal?" ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    */

    tf = IsTreeEqual( temp1, temp2 ) ; 
  
    if ( tf )
      re_node->content = "#t" ;
    else
      re_node->content = "nil" ;
  
    return re_node ;
  } // else if
  
  else if ( ans->left != NULL && ( ans->left->content == "and" || ans->left->content == "or" ) ) {

    if ( Num_argument( ans ) < 2 ) {
      if ( ans->left->content == "and" ) 
        throw new Exception7( "and" ) ;
      if ( ans->left->content == "or" ) 
        throw new Exception7( "or" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * head = new Tree() ;
    Tree * re_temp = new Tree() ; // �̫�n�^�Ǫ�node 
    temp->pro = true ;
    re_temp->pro = true ;
    head->pro = true ;
    head = ans ; // ���V�@�}�l 
    temp->content = "" ;
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        temp = Which( ans->left ) ;
      
      if ( temp->content == "nil" && head->left->content == "and" ) { // �^�ǲĤ@��nil 
        re_temp->content = "nil" ;
        return re_temp ;
      } // if
    
      if ( temp->content != "nil" && head->left->content == "or" ) { // �^�ǲĤ@�Ӥ��Onil 
        return temp ;
      } // if
      
      isDef_exist = false ;
    } // while
    
    return temp ; // ���M��ӳ��O�^�ǳ̫� 
  } // else if 
  
  else if ( ans->left != NULL && ans->left->content == "if" ) {

    if ( Num_argument( ans ) != 2 && Num_argument( ans ) != 3 ) {
      throw new Exception7( "if" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * tmp = new Tree() ;
    Tree * n_temp = new Tree() ;
    Tree * re_temp = new Tree() ; // �̫�n�^�Ǫ�node 
    temp->pro = true ;
    n_temp->pro = true ;
    tmp->pro = true ;
    re_temp->pro = true ;
    n_temp = ans ; // ���O�d�@�}�l��ans���� ���Vleft���N��O�L��content�N�Ostring>?������ 
    temp->content = "" ;
    tmp->content = "" ;
    int arguments = 1 ;
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL )
        gattempt = false ; // �i���i�L? 
      temp = Which( ans->right->left ) ;
    } // if

    if ( temp->content == "nil" ) {
      temp->content = "nil" ;
    } // if
  
    else {
      temp->content = "#t" ;
    } // else
    
    if ( ans->right != NULL ) {
      ans = ans->right ;  
    } // if
    
    isDef_exist = false ;
    if ( ans->right->content != "nil" ) { // ����nil�N��w���� error
      arguments++ ; // 2
      ans = ans->right ;
      if ( temp->content == "#t" ) {
        
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
          if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
            tmp = Which( gDef_name[i].point ) ;
            isDef_exist = true ;
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
             ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
          int num = ErrorNum( ans->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) {
          if ( ans->left->left != NULL && ans->left->right != NULL )
            gattempt = false ;
          tmp = Which( ans->left ) ;
        } // if

        return tmp ;
      } // if 
      
      else if ( temp->content == "nil" ) {
        ans = ans->right ;
        if ( ans->content == "nil" ) {
          gErrorTree = n_temp ;
          ErrorMessage( 8 ) ;
        } // if
        
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
          if ( ( ans->left->content == gDef_name[i].content ) ) {
            tmp = Which( gDef_name[i].point ) ;
            isDef_exist = true ;
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->left->content ) && 
             ! IsFloat( ans->left->content ) &&
             ! IsString( ans->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
          int num = ErrorNum( ans->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) {
          if ( ans->left->left != NULL && ans->left->right != NULL )
            gattempt = false ;
          tmp = Which( ans->left ) ;
        } // if

        return tmp ;
      } // else if
    } // if

    return re_temp ;
  } // else if

  else if ( ans->left != NULL && ans->left->content == "cond" ) { // �ݾ𪺵��c�̷� 
    if ( Num_argument( ans ) < 1 ) {
      gErrorTree = ans ;
      ErrorMessage( 9 ) ; 
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * lasttemp = new Tree() ; // �O��̫�@�ӿ�X���W�@�� �]���ȳ̫�@�ӷ|�����~ 
    Tree * re_temp = new Tree() ; // �̫�n�^�Ǫ�node 
    Tree * walk = new Tree() ;
    Tree * check = new Tree() ; // error
    Tree * error = new Tree() ; // error
    temp->pro = true ;
    re_temp->pro = true ;
    walk->pro = true ;
    temp->content = "" ;
    walk->content = "" ;
    walk = ans ;
    re_temp = ans ;
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 

    error = ans->right ;
    for ( int i = 0 ; error->content != "nil" ; error = error->right ) { // �]��ʾ�ݦ��L���~ 
    
      check = error->left ;
      if ( check->left == NULL ) { // �@�}�l�����䪺����@�w�n���F�� ���|�Onil 
        gErrorTree = ans ;
        ErrorMessage( 9 ) ;
      } // if
    
      if ( check->right->content == "nil" ) { // ( ����nil�N��L���i�঳tf ) 
        gErrorTree = ans ;
        ErrorMessage( 9 ) ;
      } // if
    
      for ( int i = 0 ; check != NULL ; check = check->right ) {
        if ( check->content != "" && check->content != "nil" ) {
          gErrorTree = ans ;
          ErrorMessage( 9 ) ;
        } // if
      } // for
    } // for

    int argument = 0 ;
    while ( walk->right->content != "nil" ) { // �ݦ��X��argument 
      argument++ ;
      walk = walk->right ;
    } // while

    // �۷��if������y ===============================================
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� �ݥL�Ot or f 
      if ( ans->right->left->left->content == gDef_name[i].content ) { // /////
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for

    if ( ! isDef_exist && ! IsInt( ans->right->left->left->content ) && 
         ! IsFloat( ans->right->left->left->content ) &&
         ! IsString( ans->right->left->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      temp = Which( ans->right->left->left ) ;
    } // if

    // ================================== 
    // �Y�Otrue 
    if ( temp->content == "#t" || ( temp->left != NULL && temp->right != NULL ) ||
         IsInt( temp->content ) || IsFloat( temp->content ) || IsString( temp->content ) ) { // �Ĥ@�ӿ�X�I 
      bool isNil = false ;
      walk = ans->right->left->right ;
      if ( walk->right->content == "nil" )
        isNil = true ;
      while ( walk->right->content != "nil" ) { // " �M���X���T���̫�@�� " ���n���q�� 
      
        if ( walk->left->left != NULL && walk->left->right != NULL ) {  
          gattempt = false ; // �o�Onon func error���P�_ 
        } // if
        
        lasttemp = Which( walk->left ) ; 
        walk = walk->right ;
      } // while
    
      if ( isNil ) { // �Y�L�@�}�l�k��N�Onil 
        if ( walk->left->left != NULL && walk->left->right != NULL ) {
          gattempt = false ; // �o�Onon func error���P�_  
        } // if
        
        lasttemp = Which( walk->left ) ; // ���N������ 
      } // if
    
      else
        lasttemp = Which( walk->left ) ;
    
      return lasttemp ;
    } // if
    
    // �ĤG���� 
    while ( ans->right->content != "nil" ) { // �Ĥ@�����S��X ���N��ĤG���� 
      ans = ans->right ; // �����k�䨫�@�� 
      argument-- ;
      if ( argument > 1 ) { // �j��@�N���O�̫�@���� 
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �ĤG���� if ���P�_�� 
          if ( ans->right->left->left->content == gDef_name[i].content ) {
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // ��즳�ܼƳQ�w�q 
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->right->left->left->content ) && 
             ! IsFloat( ans->right->left->left->content ) &&
             ! IsString( ans->right->left->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
          int num = ErrorNum( ans->right->left->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
          temp = Which( ans->right->left->left ) ;
      } // if

      // temp->content == "#t" || ( temp->left != NULL && temp->right != NULL ) ||
      //           IsInt( temp->content ) || IsFloat( temp->content ) || IsString( temp->content )
      if ( temp->content != "nil" && argument > 1 ) { // �ĤG����true 
        bool isNil = false ;
        walk = ans->right->left->right ;
        if ( walk->right->content == "nil" ) // ��W���ۦP 
          isNil = true ;
        while ( walk->right->content != "nil" ) { // �M���X�̫�@�� 
        
          if ( walk->left->left != NULL && walk->left->right != NULL ) {
            gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
          } // if
      
          lasttemp = Which( walk->left ) ;
          walk = walk->right ;
        } // while
           
        if ( isNil ) {
          if ( walk->left->left != NULL && walk->left->right != NULL ) {
            gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
          } // if
        
          lasttemp = Which( walk->left ) ;
        } // if   
        
        else
          lasttemp = Which( walk->left ) ;
        
        return lasttemp ;
      } // if
      
      else if ( argument == 1 ) { // �L�Xdefault else ( �i��n�b�o�䰵no return value )
        bool lastElse = false ;
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �ĤG���� if ���P�_�� 
          if ( ans->right->left->left->content == gDef_name[i].content ) {
            if ( ans->right->left->left->content == "else" )
              lastElse = true ;
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // ��즳�ܼƳQ�w�q 
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->right->left->left->content ) && 
             ! IsFloat( ans->right->left->left->content ) &&
             ! IsString( ans->right->left->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
          int num = ErrorNum( ans->right->left->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) // �S��� ��������~�򻼰j 
          temp = Which( ans->right->left->left ) ;
        
        if ( temp->content != "nil" || lastElse ) {
          bool isNil = false ; 
          walk = ans->right->left->right ;
          if ( walk->right->content == "nil" ) // ��W���ۦP 
            isNil = true ;
              
          while ( walk->right->content != "nil" ) { // ���ӭn�ݥ����X��argument �M���X�̫�@�� 
            if ( walk->left->left != NULL && walk->left->right != NULL ) {
              gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
            } // if
          
            temp = Which( walk->left ) ; // ���O���� �����n�������� 
            walk = walk->right ;
          } // while
            
          if ( isNil ) {
            if ( walk->left->left != NULL && walk->left->right != NULL ) {
              gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
            } // if
        
            temp = Which( walk->left ) ;
          } // if   
            
          else
            temp = Which( walk->left ) ;
              
          for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // �ĤG���� if ���P�_�� 
            if ( temp->content == gDef_name[i].content ) {
              temp = Which( gDef_name[i].point ) ;
            } // if
          } // for
              
          return temp ;
        } // if
    
        else {
          // �̫�@�ӳ����~���N�Ono return value 
          gErrorTree = re_temp ; // ���u���@�檺cond ( no return value ) 
          ErrorMessage( 10 ) ;
        } // else
      } // else if
      
      isDef_exist = false ;
    } // while
    
    gErrorTree = re_temp ; // ���u���@�檺cond ( no return value ) 
    ErrorMessage( 10 ) ;
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "begin" ) {
    if ( Num_argument( ans ) < 1 ) {
      throw new Exception7( "begin" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * re_temp = new Tree() ; // �̫�n�^�Ǫ�node 
    Tree * walk = new Tree() ;
    temp->pro = true ;
    re_temp->pro = true ;
    walk->pro = true ;
    temp->content = "" ;
    walk->content = "" ;
    walk = ans ;
    bool isDef_exist = false ; // �d�ݫ᭱��tree �O�_���w�w�q���ܼ� 

    int argument = 0 ;
    while ( walk->right->content != "nil" ) { // �ݦ��X��argument 
      argument++ ;
      walk = walk->right ;
    } // while

    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
      if ( ans->right->left->content == gDef_name[i].content ) { // /////
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
        gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
      } // if

      temp = Which( ans->right->left ) ;
    } // if

    while ( ans->right->content != "nil" ) { // �h��argument ���̫�@�� 
      ans = ans->right ;
      argument-- ;
      if ( argument > 1 ) {
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
          if ( ans->right->left->content == gDef_name[i].content ) {
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // ��즳�ܼƳQ�w�q 
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->right->left->content ) &&
             ! IsFloat( ans->right->left->content ) &&
             ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
          int num = ErrorNum( ans->right->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
          if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
            gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
          } // if

          temp = Which( ans->right->left ) ;
        } // if
      } // if
      
      if ( argument == 1 ) {
    
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
          if ( ans->right->left->content == gDef_name[i].content ) {
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // ��즳�ܼƳQ�w�q 
          } // if
        } // for
     
        if ( ! isDef_exist && ! IsInt( ans->right->left->content ) &&
             ! IsFloat( ans->right->left->content ) &&
             ! IsString( ans->right->left->content ) ) { // �N��N�O�S�Q�w�q�L��sym 
          int num = ErrorNum( ans->right->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
     
        if ( ! isDef_exist ) { // �S��� ��������~�򻼰j 
          if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
            gattempt = false ; // cons���k�����i��O�@�� ( 1 2 3 ) �n���B�z 
          } // if

          temp = Which( ans->right->left ) ;
        } // if

        return temp ;
      } // else if
      
      isDef_exist = false ;
    } // while
    
    return temp ; // ��u���@��argument �q�o�̿�X 
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "let" ) {
    Tree * temp1 = new Tree() ; // x
    Tree * temp2 = new Tree() ; // y
    Tree * re_temp = new Tree() ; // �̫�n�^�Ǫ�node 
    temp1->pro = true ;
    temp2->pro = true ;
    re_temp->pro = true ;
    temp1->content = "" ;
    temp2->content = "" ;
    bool isDef_exist = false ; 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
      if ( ans->right->left->left->left->content == gDef_name[i].content ) { // /////
        temp1 = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // ��즳�ܼƳQ�w�q
      } // if
    } // for

    if ( ! isDef_exist ) {
      temp1 = Which( ans->right->left->left->right->left ) ; // x
      gLocal_name.push_back( temp1 ) ;
    } // if
    
    isDef_exist = false ; 
    if ( ans->right->left->right->content != "nil" ) { // ���ĤG��local�ܼ� 
      
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
        if ( ans->right->left->right->left->content == gDef_name[i].content ) { //
          temp2 = Which( gDef_name[i].point ) ;
          isDef_exist = true ; // ��즳�ܼƳQ�w�q
        } // if
      } // for
    
      if ( ! isDef_exist ) {
        temp2 = Which( ans->right->left->right->right->left ) ; // y
        gLocal_name.push_back( temp1 ) ;
      } // if
    } // if
  
  } // else if
  
  else if ( ans->left != NULL &&
            ( ans->left->content == "lambda" ) ) {
              
    Tree * temp1 = new Tree() ; // x
    Tree * temp2 = new Tree() ; // y
    Tree * walk = new Tree() ; // y
    temp1->pro = true ;
    temp2->pro = true ;
    temp1->content = "" ;
    temp2->content = "" ;
    bool isDef_exist = false ; 
    
    if ( gproLambda ) {
      
      
      
      temp1->content = "Lambda" ; // pro  
      return temp1 ;
    } // if
    
    else {
      
      
      
      
    } // else 
    
    if ( ans->right->left->content == "nil" ) { // �ݦ��S���Ѽ� �Y�S�� 
      temp1 = Which( ans->right->right->left ) ;
      walk = ans->right->right ;
      while ( walk->right->content != "nil" ) {
        Tree * tmp = new Tree() ;
        tmp->content = "" ;
        walk = walk->right ;
        tmp = Which( walk->left ) ;
        if ( tmp->content > temp1->content ) {
          temp1 = tmp ;
        } // if
        
      } // while

      return temp1 ;
    } // if
    
    else { // �ȥB�{�w�L�u���@�ӰѼ� 
    // Lambda( ans, ans->right->left->left->content ) ;
    } // else
    
    
    return temp1 ;
  } // else if 
  
  else if ( ans->content != "" ) { // ��W�U��ն��� 
    ans->pro = true ; // 2-5 new && Profunction(ans) ------------|     
    Tree * temp = new Tree() ;
    temp = ans ;
    if ( ! gattempt ) { // attempt error
      int num = ErrorNum( temp->content, NULL ) ; // ///////
      ErrorMessage( num ) ;
    } // if

    return ans ;
  } // else if
  
  else {
    if ( gattempt && ! gDef ) { // throw non func 

      if ( ans->left != NULL ) {
        if ( IsFunction( ans->left->content ) ) {
          gErrorTree = ans ; 
          gErrorTree = Which( ans->left ) ;
          int num = ErrorNum( gErrorStr, gErrorTree ) ; // ///////
          ErrorMessage( num ) ;
        } // if
        
        else {
          bool isDef = false ;
          for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if ���P�_�� 
            if ( ans->left->content == gDef_name[i].content ) {
              isDef = true ;
            } // if
          } // for
          
          if ( ! isDef && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) && 
               ! IsString( ans->left->content ) ) {
            int num = ErrorNum( ans->left->content, NULL ) ;
            ErrorMessage( num ) ;
          } // if
        } // else
        
        if ( ans->left->left != NULL ) {
          if ( IsFunction( ans->left->left->content ) || ans->left->left->content == "" ) {
            Tree * temp = ans->left ; 
            gErrorTree = Which( temp ) ;
            ErrorMessage( 12 ) ;
          } // if
        } // if 
      } // if
    } // if
    
    ans->pro = true ;
    return ans ; // 2-5 new add
  } // else

  return ans ;
} // Which()

void ReColLine() { // reset these global var 
  gcol = 1 ;
  gcol_before = 1 ;
  gline = 1 ;
  gline_before = 1 ;
} // ReColLine()

int main() {
  
  Token token ;
  string str ;
  char ch_peek = '\0' ;
  char tmp = '\0' ;
  char temp = '\0' ;  
  char ch[100] ;
  bool end = false ;
  int uTestNum = 0 ; 
  int count = 0 ;
  cin >> uTestNum ;
  // if ( uTestNum == 1 )
  //  exit( 0 ) ;
  cout << "Welcome to OurScheme!" << endl << endl << "> " ;
  
  while ( ! end ) { // until user entered (exit) or eof encounted
    try {  // 
      Tree * ans = new Tree() ;
      Tree * n_ans = new Tree() ;
      ans->pro = true ;
      n_ans->pro = true ;
      int i = 0 ;
      int space = 1 ;
      bool n_node = false ;
      bool not_def_error = false ;
      bool attempt_error = false ;
      Sexp( token ) ; //  ( 1 . 2 )
      gcol = 1 ;
      gcol_before = 1 ;

      ans = Build2( token, i, n_node ) ;
      IsExit( ans ) ; 

      if ( token.tokenlist.size() != 1 ) { // ��·Q�Ldefine�����ܼƥi�H�����L 
        n_ans = Which( ans ) ;
        gIsNew_tree = true ;
      } // if

      if ( ! gIs_Define ) { // �S�n��define���ʧ@�~�i 

        if ( n_ans->left != NULL && n_ans->right != NULL ) // token.tokenlist.size() != 1 origin
          cout << "( " ; // ���� car '(1 2 3 ) �����G ���G�u���@��node �G���ΦL���A�� 
        else
          gonly_one = true ;

        if ( token.tokenlist.size() != 1 ) // �]���ndefine �ҥHsize���|�u��1 
          Print( n_ans, space ) ;
        else { // �B�z�L define�n���� ����while����� 
          while ( i < gDef_name.size() ) { // �䤣����Ф���i��n�Lerror 
            if ( ans->content == gDef_name[i].content ) {
              if ( gDef_name[i].point->left != NULL && gDef_name[i].point->right != NULL ) 
                cout << "( " ; // �N���ӫ��Ъ�tree���u���@��node�Ӥw
              Print( gDef_name[i].point, space ) ;
              not_def_error = true ; // ���Q�w�q�L���ܬ�true 
            } // if
        
            i++ ;
          } // while
          
          if ( ! not_def_error && ! IsInt( ans->content ) && ! IsFloat( ans->content ) && 
               ! IsString( ans->content ) && ans->content != "nil" && ans->content != "#t" &&
               ! ProFunction( ans ) ) {
            throw new Exception6( ans->content ) ; // �S�Q�w�q�L���ܼƭn��error ( unbounded ) 
          } // if
        } // else
    
        if ( ( ! gIsNew_tree && ( IsInt( ans->content ) || IsFloat( ans->content )
                                  || IsString( ans->content ) || ans->content == "nil" 
                                  || ans->content == "#t" ) )
             || ProFunction( ans ) ) {
          ans->pro = true ;
          Print( ans, space ) ; // only one int or float but not symbol 
        } // if

        gPrintDone = true ;
        if ( gPrintDone ) {
          ch_peek = cin.peek() ;
          while ( ch_peek == '\t' || ch_peek == ' ' ) {
            temp = Getchar() ;
            ch_peek = cin.peek() ;
          } // while
      
          if ( ch_peek == ';' ) {
            gSexpDone = true ;
          } // if
      
          else
            gSexpDone = false ;
      
          if ( ch_peek == '\n' ) {
            temp = Getchar() ;
            gline = 1 ;
            gline_before = 1 ;
          } // if
      
          gline = 1 ;
          gline_before = 1 ;
        } // if
      } // if
    } // try()
    
    catch( Exception1 * e ) {
      GetToken( token ) ;
      cin.getline( ch, 100 ) ;
      cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
      cout << gline_before << " Column " << gcol_before << " is >>" ;
      cout << e->mMsg ;
      cout << "<<" << endl ;
      ReColLine() ;
      gSexpDone = false ;
    } // catch()
    
    catch( Exception2 * e ) {
      GetToken( token ) ;
      cin.getline( ch, 100 ) ;
      cout << "ERROR (unexpected token) : ')' expected when token at Line " ;
      cout << gline_before << " Column " << gcol_before << " is >>" ;
      cout << e->mMsg ;
      cout << "<<" << endl ;
      ReColLine() ;
      gSexpDone = false ;
    } // catch()
    
    catch( Exception3 * e ) {
      cin.getline( ch, 100 ) ;
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
      cout << gline_before << " Column " << gcol_before ;
      cout << endl ;
      ReColLine() ;
    } // catch()
    
    catch( Exception4 * e ) { // �J��eof�����X 
      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
      end = true ;
    } // catch()
    
    catch( Exception5 * e ) { // �ݨ� (exit) ���X 
      end = true ;
    } // catch()
    
    catch( Exception6 * e ) { // ////////
      cout << "ERROR (unbound symbol) : " << e->mMsg << endl ;
      ReColLine() ;
      tmp = cin.peek() ;
      while ( tmp == ' ' || tmp == '\t' ) {
        Getchar() ;
        tmp = cin.peek() ;
      } // while

      if ( tmp == ';' ) {
        cin.getline( ch, 100 ) ;
      } // if
 
      if ( tmp == '\n' ) {
        tmp = getchar() ;
      } // if

      else {
        gline = 1 ;
        gline_before = 1 ;
      } // else
    } // catch()
    
    catch( Exception7 * e ) { // �٭n�[�F�� 
      cout << "ERROR (incorrect number of arguments) : " << e->mMsg << endl ;
      ReColLine() ;
      tmp = cin.peek() ;
      while ( tmp == ' ' || tmp == '\t' ) {
        Getchar() ;
        tmp = cin.peek() ;
      } // while
    
      if ( tmp == '\n' ) {
        tmp = getchar() ;
      } // if

      else {
        gline = 1 ;
        gline_before = 1 ;
      } // else
    } // catch()
    
    catch( Exception8 * e ) { // the same
      if ( ! gattempt ) 
        cout << "ERROR (attempt to apply non-function) : " << e->mMsg << endl ;
      ReColLine() ;
      tmp = cin.peek() ;
      while ( tmp == ' ' || tmp == '\t' ) {
        Getchar() ;
        tmp = cin.peek() ;
      } // while
    
      if ( tmp == '\n' ) {
        tmp = getchar() ;
      } // if

      else {
        gline = 1 ;
        gline_before = 1 ;
      } // else
    } // catch()
    
    catch( Exception9 * e ) {
      cout << "ERROR (level of CLEAN-ENVIRONMENT)" << endl ;
      ReColLine() ;
      tmp = cin.peek() ;
      while ( tmp == ' ' || tmp == '\t' ) {
        Getchar() ;
        tmp = cin.peek() ;
      } // while
    
      if ( tmp == '\n' ) {
        tmp = getchar() ;
      } // if

      else {
        gline = 1 ;
        gline_before = 1 ;
      } // else
    } // catch()
    
    catch( Exception10 * e ) {
      ReColLine() ;
      tmp = cin.peek() ;
      while ( tmp == ' ' || tmp == '\t' ) {
        Getchar() ;
        tmp = cin.peek() ;
      } // while
    
      if ( tmp == '\n' ) {
        tmp = getchar() ;
      } // if

      else {
        gline = 1 ;
        gline_before = 1 ;
      } // else

    // �����D ( + 3 3 nil ) ) �|�� 
    } // catch()
    
    token.curstr = "" ;
    token.tokenlist.clear() ;
    g_bpeek = false ;  
    gonly_one = false ;
    gPrintDone = false ;
    gend_of_file = false ;
    gIs_Define = false ;
    gIsNew_tree = false ;
    gattempt = false ;
    gnon = false ;
    gProPrint = false ;
    gDef = false ;
    gfirst_which = 0 ;

    if ( ! end )
      cout << endl << "> " ; // last, vector should be clean
  } // while

  cout << endl << "Thanks for using OurScheme!" ;
} // main()
