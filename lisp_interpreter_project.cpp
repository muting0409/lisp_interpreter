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
bool gonly_one = false ; // 當輸入只有一個時設成true 
bool gend_of_file = false ;
bool gSexpDone = false ; // 檢查Sexp做完沒 用在gettoken ";" 
bool gPrintDone = false ; // 檢查那一行有無印出 有印出代表那一行已經結束 下一行line不用++ 
bool gIs_Define = false ; // 除了用在define 還用在clean environment上 只是為了clean之後不要印出來 重新再loop一次 
bool gIsNew_tree = false ; // 檢查是否進入which 這個function 用來區分 1 和 car '( 1 2 3 ) 用的 
bool gpro = true ;
bool gProPrint = false ; // if 錯了的時候 裡面的func不能印pro 
bool gnon = false ;
bool gattempt = false ; // 用來看是不是attempt error
bool gproLambda = false ;
bool gDef = false ; // 看有沒有被定義 有則true 只是寫死 擋而已 
bool gWhite = false ;
int gfirst_which = 0 ; // 用來看clean-environ 是不是單獨的 大於1代表不是單獨的 ( clean ... ) 才是正確 
string gErrorStr = "" ; // 用來存錯誤的那個token 

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
  Tree *point ; // 用來印define的tree 
  bool pro ; // 預設為true 建樹的時候遇到atom會變成false 有evaluate才會重新變成true 主要處理procedure問題 
} ;

vector<Tree> gDef_name ; // 存著tree結構的指標 指標都指向被define過的一棵樹 ( 用在which這個function的define ) 
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
  string temp = "" ; // 暫時放切好的token 
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
          
        while ( ! bnum ) { // true代表數字開頭token切完 
            
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
          } // else if SYMBOL的條件 SYMBOL不包含這些 
            
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
          } // else if SYMBOL的條件 SYMBOL不包含這些  
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
            
        } // while
      } // else if
        
      else if ( ch_peek == ')' ) { // 原版本這三行 
        ch = Getchar() ;
        temp += ch ;
        done = true ;
      } // else if
        
      else if ( ch_peek == '(' ) { // (  ) 的case 要把它合併在一起 
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
          } // else if SYMBOL的條件 SYMBOL不包含這些  
            
          else {
            ch = Getchar() ;
            temp += ch ;
            ch_peek = cin.peek() ;
          } // else
            
        } // while
      } // else if
        
      else if ( ch_peek == '_' ) { // 全部底線的case尚未考慮 
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
          } // else if SYMBOL的條件 SYMBOL不包含這些  
            
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
          } // else if SYMBOL的條件 SYMBOL不包含這些  
          
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
        
      else if ( ch_peek == '"' ) { // 第三種錯誤 沒有第二個雙引號 
        ch = Getchar() ;
        bool bquote = false ;
        temp += ch ;
        ch_peek = cin.peek() ;
        while ( ! bquote ) {
          if ( ch_peek != '\n' ) { // 不是換行 
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
            
          else { // 沒看到第二個雙引號 
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
        if ( ! gSexpDone ) { // 這邊若跑完sexp就不用+1
          gline++ ;
          gline_before++ ;
        } // if
          
        gcol = 1 ; // 換行 col = 1 
        gcol_before = 1 ;
        ch_peek = cin.peek() ;
        SkipWhiteSpace( ch_peek ) ; // 會吃到white space 所以先跳過 
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
          } // else if SYMBOL的條件 SYMBOL不包含這些  
            
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
        
      else { // 一些奇怪的symbol 
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
  if ( str[index] == '"' ) { // 第一個是雙引號 
    
    index++ ;
    while ( str[index] != '"' ) {
      
      if ( index != ( str.size() - 1 ) ) // 
        index++ ;
      else // size跟index一樣大卻還沒找到 " 
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
  
  if ( str[index] == '.' ) { // 第一個char是小數點 .34 
  
    if ( str.size() != 1 ) { // 不是只有一個點 (保險) 
      index++ ;
      
      if ( index >= str.size() ) // 防止 -. 
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
  
  else { // 第一個char不是小數點 eg 12.34 
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
    
    else // 同時判斷第一個char是不是數字and 數字過後是不是小數點 
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
    token.tokenlist.push_back( token.curstr ) ; // 沒錯就push進去 
  } // if
  
  else if ( token.curstr == "(" ) {
    GetToken( token ) ;
    token.tokenlist.push_back( token.curstr ) ; // 沒錯就push進去 
    Sexp( token ) ;

    PeekToken( token ) ; // 2
    while ( Atom( token.curstr ) || token.curstr == "(" || token.curstr == "'" ) {
      Sexp( token ) ;
      PeekToken( token ) ; // 2
    } // while

    if ( token.curstr == "." ) {
      GetToken( token ) ;
      token.tokenlist.push_back( token.curstr ) ; // 沒錯就push進去 
      Sexp( token ) ;
    } // if
    
    PeekToken( token ) ;
    if ( token.curstr == ")" ) {
      GetToken( token ) ;
      token.tokenlist.push_back( token.curstr ) ; // 沒錯就push進去 
    } // if
    
    else { //
      throw new Exception2( token.curstr ) ;
    } // else
    
  } // else if
  
  else if ( token.curstr == "'" ) {
    GetToken( token ) ;
    token.tokenlist.push_back( "(" ) ;
    token.tokenlist.push_back( "quote" ) ; // 沒錯就push進去 
    Sexp( token ) ;
    token.tokenlist.push_back( ")" ) ;
  } // else if
  
  else {
    throw new Exception1( token.curstr ) ; 
  } // else
} // Sexp()

Tree * Build2( Token &token, int &i, bool new_node ) {

  Tree * node = new Tree() ; // 建node 
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

bool ProFunction_str( string str ) { // 差別只是參數是string 
  
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
  
  if ( ProFunction( ans ) && ans->pro && ! gProPrint ) { // 單一一個pro funcion 
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
        if ( i + 1 < ans->content.size() && ans->content[i+1] == 'n' ) { // 現在是 \ 下一個是n時
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
  
  else { // 剩下為symbol 
    cout << ans->content ;
  } // else
} // PrintType()
  
void Print( Tree * ans, int &space ) {
  bool isleft = true ;
  if ( ans->content != "" && ans->content != "nil" ) { // if encounter atom then print 
    PrintType( ans ) ;
    cout << endl ;
  } // if
  
  else if ( ans->content == "nil" && gonly_one ) { // 當輸入為1個 and 為nil時 
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
  if ( IsFunction( str ) ) // 看到這些function 給過 
    return -1 ;

  if ( ! gattempt && ( IsInt( str ) || IsFloat( str ) || IsString( str ) ) ) { // 這邊應該要輸出tree 
    gErrorStr = str ;
    return 2 ;
  } // if
  
  if ( gattempt && isTree ) { // non func是tree 
    cout << "ERROR (attempt to apply non-function) : " ;
    int space = 1 ;
    cout << "( " ;
    Print( temp, space ) ;
    return 2 ;
  } // if

  if ( str == "" || IsInt( str ) || IsFloat( str ) || IsString( str ) || str == "nil" || 
       str == "#t" ) {
    return -1 ; // 這些不算error 
  } // if
  
  int vector_has = 0 ;
  bool isDef = false ;
  int j = 0 ;
  for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 那個變數有被定義過 且 是那些function 
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
  
  if ( vector_has == 0 ) { // 那個function是隨便打的 
    gErrorStr = str ;
    return 1 ; // eg ( conss 3 5 )....
  } // if
  
  else if ( vector_has == 2 ) { // non func 不是tree 
    if ( isDef )
      gErrorStr = gDef_name[j].point->content ;
    else
      gErrorStr = str ;
    
    return 2 ;
  } // else if ( define a 5 ) , ( a 5 5 ) 要印那個node的content 
  
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
    
    if ( gErrorTree->content == "" ) { // 是一棵樹 
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
    
    if ( gErrorTree->content == "" ) { // 是一棵樹 
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
  
  for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 那個變數有被定義過 且 是那些function 
    if ( ans->content == gDef_name[i].content ) {
      if ( ProFunction( gDef_name[i].point ) ) {
        ans->content = gDef_name[i].point->content ;
      } // if
    } // if
  } // for
  
  if ( ans->left != NULL && ( ans->left->content == "quote" ) ) {
    if ( Num_argument( ans ) != 1 ) { // 好幾個參數 
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
    gattempt = true ; // 有進來代表function的名字是正確的 
    if ( Num_argument( ans ) != 2 ) {
      throw new Exception7( "cons" ) ;
    } // if
  
    Tree * temp = new Tree() ;
    temp->pro = true ;
    temp->content = "" ;
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gnon = true ; // 遇到變數的時候 到nonlist會錯 所以多加一個條件 
        temp->left = Which( gDef_name[i].point ) ;
        gnon = false ; // 用完關掉 
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
        gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
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
    
    if ( ! isDef_exist ) { // 看有無被定義過 
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
    if ( Num_argument( ans ) < 0 ) { // 好幾個參數 
      throw new Exception7( "list" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // 要return的指標 
    Tree * temp = new Tree() ;
    temp->left = NULL ;
    temp->right = NULL ;
    re_node->left = NULL ;
    re_node->right = NULL ;
    temp->pro = true ;
    re_node->pro = true ;
    re_node = temp ; // 先指到最一開始 
    temp->content = "" ;
    if ( ans->right != NULL ) {
      ans = ans->right ;  
    } // if
    
    if ( ans->content == "nil" ) { // ( list ) 會輸出nil的情況 
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
    
    if ( ! isDef_exist ) { // 看有無被定義過
      int num = ErrorNum( ans->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) {
      if ( ans->left->left != NULL && ans->left->right != NULL )
        gattempt = false ;
      temp->left = Which( ans->left ) ;
    } // if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
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
      
      if ( ! isDef_exist ) { // 看有無被定義過 
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

    temp->right = Which( ans->right ) ; // 出迴圈代表右邊剩下nil 再一次遞迴即可接上nil 
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
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist )
      temp = Which( ans->right->left ) ;

    if ( temp->content != "" ) { // 表示只有一個node 
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
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist )
      temp = Which( ans->right->left ) ;
      
    if ( temp->content != "" ) { // 表示只有一個node 
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
  
    if ( Num_argument( ans ) != 2 ) { // 好幾個參數 
      gErrorTree = ans ;
      ErrorMessage( 3 ) ;
    } // if
  
    if ( IsFunction( ans->right->left->content ) || IsInt( ans->right->left->content ) ||
         IsFloat( ans->right->left->content ) || IsString( ans->right->left->content ) ||
         ans->right->left->content == "#t" || ans->right->left->content == "nil" ) {
      gErrorTree = ans ; // 要被定義的變數是那些func 
      ErrorMessage( 3 ) ; 
    } // if
  
    gattempt = true ;
    Tree temp ;
    temp.pro = true ;
    temp.content = ans->right->left->content ; // tree的名字 
    bool reDef = false ; // 查詢此變數是否被重新define
    bool isDef_exist = false ; 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 看樹的名字有無被定義過 
      if ( temp.content == gDef_name[i].content ) { // 找到被定義過 eg ( define "a" b ) 
        for ( int j = 0 ; j < gDef_name.size() ; j++ ) { // 再看後面是否是已定義過的tree  eg ( define a "b" ) 
          if ( ans->right->right->left->content == gDef_name[j].content ) { // 找到後面的tree 
            gDef_name[i].point = gDef_name[j].point ; // 前面的變數指向後面的 eg a point to b 
            isDef_exist = true ; 
          } // if
        } // for
        
        if ( ! isDef_exist )
          gDef_name[i].point = Which( ans->right->right->left ) ; // 若有被定義過 則該指標重新指向新樹 

        reDef = true ; 
      } // if
    } // for
    
    if ( ! reDef ) { // 若沒被定義過 則跑一次遞迴 然後point push進vector
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
    gIs_Define = true ; // 用來不用print tree用的 
    return 0 ;
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "clean-environment" ) {

    if ( gfirst_which > 5 ) { // 因為前面有遞迴要跑 所以變成要大於5 
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
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
      temp = Which( ans->right->left ) ;
    } // if
    
    if ( temp->left == NULL && temp->right == NULL ) {
      // IsInt( temp->content ) || IsFloat( temp->content ) || IsString( temp->content )
      n_temp->content = "#t" ;
    } // if 只有一個node就是atom 但可能會有問題 
    
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
    Tree * temp = new Tree() ; // 接一棵樹才是true 反之就是atom的一個node
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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
    Tree * temp = new Tree() ; // 最右邊是nil才是true 
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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
    Tree * temp = new Tree() ; // 右邊的左邊是nil 
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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
    Tree * temp = new Tree() ; // 右邊的左邊是int
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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

    Tree * temp = new Tree() ; // 右邊的左邊是實數或number ( is-real IFF is-number ) 
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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

    Tree * temp = new Tree() ; // 右邊的左邊是string
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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

    Tree * temp = new Tree() ; // 右邊的左邊是bool
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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

    Tree * temp = new Tree() ; // 右邊的左邊是symbol
    Tree * n_temp = new Tree() ; 
    temp->content = "" ;
    temp->pro = true ;
    n_temp->pro = true ;
    gattempt = true ; 
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        gDef = true ;
        temp = Which( gDef_name[i].point ) ;
        gDef = false ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 看有無被定義過 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
        gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
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
    Tree * re_node = new Tree() ; // 要return的指標 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 0 ;
    float f_res = 0.0 ;
    char *ch = new char[100] ; // 用來裝 sprintf的東西 
    bool float_type = false ; // 若是true 則輸出的時候要用float型態 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 用來找變數裡面的東西 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // 有 則接起來 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) {
      int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
      ErrorMessage( num ) ;
    } // if

    if ( ! isDef_exist ) // 只是一般的東西 
      temp = Which( ans->left ) ;
    
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = "+" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // 看接的東西的型態是什麼 
      f_res += atoi( temp->content.c_str() ) ; // 先都用float型態裝 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // 變true之後 就不會變回false 因為最後直接要用float印 
      f_res += atof( temp->content.c_str() ) ;
    } // else if
      
    else { // 這邊會出現error 因為那個變數裝的不是int or float 
    } // else
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 一樣是找變數裡面的東西 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
    
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) {
        int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
        ErrorMessage( num ) ;
      } // if
    
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // 表示只有一個node 
        gErrorTree = tmp ;
        gErrorStr = "+" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // 同上面 
        f_res += atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        f_res += atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // 這邊會出現error 因為那個變數裝的不是int or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // 是true 則直接轉成float 
      sprintf( ch, "%f", f_res ) ;
    else { // 代表是要印int型態 
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
    Tree * re_node = new Tree() ; // 要return的指標 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 0 ;
    float f_res = 0.0 ;
    char *ch = new char[100] ; // 用來裝 sprintf的東西 
    bool float_type = false ; // 若是true 則輸出的時候要用float型態 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 用來找變數裡面的東西 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // 有 則接起來 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) { 
      int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // 只是一般的東西 
      temp = Which( ans->left ) ;
  
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = "-" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // 看接的東西的型態是什麼 
      f_res += atoi( temp->content.c_str() ) ; // 先都用float型態裝 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // 變true之後 就不會變回false 因為最後直接要用float印 
      f_res += atof( temp->content.c_str() ) ;
    } // else if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 一樣是找變數裡面的東西 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { 
        int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // 表示只有一個node 
        gErrorTree = tmp ;
        gErrorStr = "-" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // 同上面 
        f_res -= atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        f_res -= atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // 這邊會出現error 因為那個變數裝的不是int or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // 是true 則直接轉成float 
      sprintf( ch, "%f", f_res ) ;
    else { // 代表是要印int型態 
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
    Tree * re_node = new Tree() ; // 要return的指標 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 1 ;
    float f_res = 1.0 ;
    char *ch = new char[100] ; // 用來裝 sprintf的東西 
    bool float_type = false ; // 若是true 則輸出的時候要用float型態 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 用來找變數裡面的東西 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // 有 則接起來 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) {
      int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // 只是一般的東西 
      temp = Which( ans->left ) ;
  
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = "*" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // 看接的東西的型態是什麼 
      f_res *= atoi( temp->content.c_str() ) ; // 先都用float型態裝 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // 變true之後 就不會變回false 因為最後直接要用float印 
      f_res *= atof( temp->content.c_str() ) ;
    } // else if
      
    else { // 這邊會出現error 因為那個變數裝的不是int or float 
    } // else
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 一樣是找變數裡面的東西 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) {
        int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // 表示只有一個node 
        gErrorTree = tmp ;
        gErrorStr = "*" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // 同上面 
        f_res *= atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        f_res *= atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // 這邊會出現error 因為那個變數裝的不是int or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // 是true 則直接轉成float 
      sprintf( ch, "%f", f_res ) ;
    else { // 代表是要印int型態 
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
    Tree * re_node = new Tree() ; // 要return的指標 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    int i_res = 1 ;
    float f_res = 1.0 ;
    char *ch = new char[100] ; // 用來裝 sprintf的東西 
    bool float_type = false ; // 若是true 則輸出的時候要用float型態 
    temp->content = "" ;
    if ( ans->right != NULL )
      ans = ans->right ;  
    
    bool isDef_exist = false ;
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 用來找變數裡面的東西 
      if ( ans->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ; // 有 則接起來 
        isDef_exist = true ;
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
         ! IsString( ans->left->content ) ) {
      int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // 只是一般的東西 
      temp = Which( ans->left ) ;
  
    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = "/" ; 
      ErrorMessage( 6 ) ;
    } // if
  
    if ( IsInt( temp->content ) ) { // 看接的東西的型態是什麼 
      int tmp = atoi( temp->content.c_str() ) ; // =========== can not divide zero ============
      f_res = tmp ; // 先都用float型態裝 
    } // if
    
    else if ( IsFloat( temp->content ) ) {
      float_type = true ; // 變true之後 就不會變回false 因為最後直接要用float印 
      float tmp = atof( temp->content.c_str() ) ;
      f_res = tmp ;
    } // else if
      
    else { // 這邊會出現error 因為那個變數裝的不是int or float 
    } // else
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 一樣是找變數裡面的東西 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) {
        int num = ErrorNum( ans->left->content, NULL ) ; // 看有無被定義過 
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // 表示只有一個node 
        gErrorTree = tmp ;
        gErrorStr = "/" ; 
        ErrorMessage( 6 ) ;
      } // if
    
      if ( IsInt( tmp->content ) ) { // 同上面 
      
        if ( atoi( tmp->content.c_str() ) == 0 )  // 除以0 
          ErrorMessage( 7 ) ; 
        f_res /= atoi( tmp->content.c_str() ) ;
      } // if
    
      else if ( IsFloat( tmp->content ) ) {
        float_type = true ;
        if ( atof( tmp->content.c_str() ) == 0.0 )
          ErrorMessage( 7 ) ; 
        f_res /= atof( tmp->content.c_str() ) ;
      } // else if
      
      else { // 這邊會出現error 因為那個變數裝的不是int or float 
      } // else
      
      isDef_exist = false ;
    } // while

    if ( float_type ) // 是true 則直接轉成float 
      sprintf( ch, "%f", f_res ) ;
    else { // 代表是要印int型態 
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
    Tree * temp = new Tree() ; // 取反向的意思 
    Tree * n_temp = new Tree() ; 
    temp->pro = true ;
    n_temp->pro = true ;
    temp->content = "" ;
    
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist ) {
      int num = ErrorNum( ans->right->left->content, NULL ) ; // 看有無被定義過 
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
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
    Tree * n_temp = new Tree() ; // 最後return的node 
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
    bool isDef_exist = false ; // 查看cons後面的tree 是否有已定義的變數 
    while ( unbound->right->content != "nil" ) {
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( unbound->right->left->content == gDef_name[i].content ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ; // 找到有變數被定義 
        } // if
      } // for
     
      if ( ! isDef_exist && ! IsInt( unbound->right->left->content ) && 
           ! IsFloat( unbound->right->left->content ) &&
           ! IsString( unbound->right->left->content ) ) { // 代表就是沒被定義過的sym 
        int num = ErrorNum( unbound->right->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
    
      if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
        temp = Which( unbound->right->left ) ;
    
      if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
           ( temp->left != NULL && temp->right != NULL ) ||
           temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
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
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
      temp = Which( ans->right->left ) ;

    if ( ( ! IsInt( temp->content ) && ! IsFloat( temp->content ) ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = str ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if

    if ( ans->right != NULL )
      ans = ans->right ; 
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" && ! isFalse ) { // 走到nil代表已走完 error
      Tree * tmp = new Tree() ;
      tmp->pro = true ;
      tmp->content = "" ;
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 一樣是找變數裡面的東西 
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;      
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ( ! IsInt( tmp->content ) && ! IsFloat( tmp->content ) ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // 表示只有一個node 
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
            ! isPro ) { // string +在一起 error
    if ( Num_argument( ans ) < 2 ) {
      throw new Exception7( "string-append" ) ;
    } // if

    gattempt = true ; 
    Tree * re_node = new Tree() ; // 要return的指標 
    Tree * temp = new Tree() ;
    temp->pro = true ;
    re_node->pro = true ;
    temp->content = "" ;
    re_node->content = "" ; // "az" + "ax" = "azax" 中間的雙引號要去掉 
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
         ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist )
      temp = Which( ans->left ) ;
    
    if ( ! IsString( temp->content ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = "string-append" ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    
    if ( IsString( temp->content ) ) {
      for ( int i = 1 ; i < temp->content.size() - 1 ; i++ )
        re_node->content += temp->content[i] ;
    } // if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        temp = Which( ans->left ) ;
        
      if ( ! IsString( temp->content ) ||
           ( temp->left != NULL && temp->right != NULL ) ||
           temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
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
    Tree * re_temp = new Tree() ; // 最後要回傳的node
    temp->pro = true ;
    n_temp->pro = true ;
    tmp->pro = true ;
    re_temp->pro = true ; 
    n_temp = ans->left ; // 先保留一開始的ans指標 指向left的意思是他的content就是string>?之類的 
    temp->content = "" ;
    tmp->content = "" ;
    bool isFalse = false ; // 如果中途有一個比較錯誤 那他就變成true 最後return nil 
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 
    
    Tree * unbound = new Tree() ;
    unbound = ans ;
    
    while ( unbound->right->content != "nil" ) {
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( unbound->right->left->content == gDef_name[i].content ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ; // 找到有變數被定義 
        } // if
      } // for
     
      if ( ! isDef_exist && ! IsInt( unbound->right->left->content ) &&
           ! IsFloat( unbound->right->left->content ) &&
           ! IsString( unbound->right->left->content ) ) { // 代表就是沒被定義過的sym 
        int num = ErrorNum( unbound->right->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
    
      if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
        temp = Which( unbound->right->left ) ;
    
      if ( ! IsString( temp->content ) ||
           ( temp->left != NULL && temp->right != NULL ) ||
           temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
        gErrorTree = temp ;
        gErrorStr = str ; // < > <= >= =
        ErrorMessage( 6 ) ;
      } // if
    
      unbound = unbound->right ;
    } // while
    
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
      temp = Which( ans->right->left ) ;
    
    if ( ! IsString( temp->content ) ||
         ( temp->left != NULL && temp->right != NULL ) ||
         temp->content == "#t" || temp->content == "nil" ) { // 表示只有一個node 
      gErrorTree = temp ;
      gErrorStr = str ; // < > <= >= =
      ErrorMessage( 6 ) ;
    } // if
    
    if ( ans->right != NULL ) {
      ans = ans->right ;  
    } // if
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" && ! isFalse ) { // 走到nil代表已走完 error
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          tmp = Which( gDef_name[i].point ) ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        tmp = Which( ans->left ) ;
    
      if ( ! IsString( tmp->content ) ||
           ( tmp->left != NULL && tmp->right != NULL ) ||
           tmp->content == "#t" || tmp->content == "nil" ) { // 表示只有一個node 
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
    temp2->content = "" ; // 第二個tree 
    temp1->content = "" ; // 第一個tree 
    tmp->content = "" ; 
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp1 = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    tmp = ans->right->left ; // 暫時指向第一棵樹 
    if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
      temp1 = Which( ans->right->left ) ;
    
    if ( ! IsInt( temp1->content ) && ! IsFloat( temp1->content ) && 
         ! IsString( temp1->content ) && temp1->content != "#t" && temp1->content != "nil" ) { // 表示只有一個node 
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
         ! IsString( ans->right->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    tmp = ans->right->right->left ; // 暫時指向第二棵樹 
    if ( ! isDef_exist2 ) // 沒找到 維持原樣繼續遞迴 
      temp2 = Which( ans->right->right->left ) ;

    if ( ! IsInt( temp2->content ) && ! IsFloat( temp2->content ) &&
         ! IsString( temp2->content ) && temp2->content != "#t" && temp2->content != "nil" ) { // 表示只有一個node 
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
  
    if ( isDef_exist && isDef_exist2 ) { // 兩個都是以定義變數 且 兩個名字都要一樣  (不一樣會輸出nil 
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
    temp2->content = "" ; // 第二個tree 
    temp1->content = "" ; // 第一個tree 
    bool tf = false ;
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp1 = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
      temp1 = Which( ans->right->left ) ;
    /*
    if ( ! IsInt( temp1->content ) && ! IsFloat( temp1->content ) && 
         ! IsString( temp1->content ) && temp1->content != "#t" && temp1->content != "nil" ) { // 表示只有一個node 
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
         ! IsString( ans->right->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist2 ) // 沒找到 維持原樣繼續遞迴 
      temp2 = Which( ans->right->right->left ) ;
    /*
    if ( ! IsInt( temp2->content ) && ! IsFloat( temp2->content ) && 
         ! IsString( temp2->content ) && temp2->content != "#t" && temp2->content != "nil" ) { // 表示只有一個node 
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
    Tree * re_temp = new Tree() ; // 最後要回傳的node 
    temp->pro = true ;
    re_temp->pro = true ;
    head->pro = true ;
    head = ans ; // 指向一開始 
    temp->content = "" ;
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 
    
    isDef_exist = false ;
    while ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
      ans = ans->right ;
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) {
        if ( ans->left != NULL && ( ans->left->content == gDef_name[i].content ) ) {
          temp = Which( gDef_name[i].point ) ;
          isDef_exist = true ;
        } // if
      } // for
      
      if ( ! isDef_exist && ! IsInt( ans->left->content ) && ! IsFloat( ans->left->content ) &&
           ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
        int num = ErrorNum( ans->left->content, NULL ) ;
        ErrorMessage( num ) ;
      } // if
      
      if ( ! isDef_exist )
        temp = Which( ans->left ) ;
      
      if ( temp->content == "nil" && head->left->content == "and" ) { // 回傳第一個nil 
        re_temp->content = "nil" ;
        return re_temp ;
      } // if
    
      if ( temp->content != "nil" && head->left->content == "or" ) { // 回傳第一個不是nil 
        return temp ;
      } // if
      
      isDef_exist = false ;
    } // while
    
    return temp ; // 不然兩個都是回傳最後 
  } // else if 
  
  else if ( ans->left != NULL && ans->left->content == "if" ) {

    if ( Num_argument( ans ) != 2 && Num_argument( ans ) != 3 ) {
      throw new Exception7( "if" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * tmp = new Tree() ;
    Tree * n_temp = new Tree() ;
    Tree * re_temp = new Tree() ; // 最後要回傳的node 
    temp->pro = true ;
    n_temp->pro = true ;
    tmp->pro = true ;
    re_temp->pro = true ;
    n_temp = ans ; // 先保留一開始的ans指標 指向left的意思是他的content就是string>?之類的 
    temp->content = "" ;
    tmp->content = "" ;
    int arguments = 1 ;
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
      if ( ans->right->left->content == gDef_name[i].content ) {
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL )
        gattempt = false ; // 可有可無? 
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
    if ( ans->right->content != "nil" ) { // 走到nil代表已走完 error
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
             ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
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
             ! IsString( ans->left->content ) ) { // 代表就是沒被定義過的sym 
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

  else if ( ans->left != NULL && ans->left->content == "cond" ) { // 看樹的結構最準 
    if ( Num_argument( ans ) < 1 ) {
      gErrorTree = ans ;
      ErrorMessage( 9 ) ; 
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * lasttemp = new Tree() ; // 記住最後一個輸出的上一個 因為怕最後一個會有錯誤 
    Tree * re_temp = new Tree() ; // 最後要回傳的node 
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
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 

    error = ans->right ;
    for ( int i = 0 ; error->content != "nil" ; error = error->right ) { // 跑整棵樹看有無錯誤 
    
      check = error->left ;
      if ( check->left == NULL ) { // 一開始的左邊的左邊一定要有東西 不會是nil 
        gErrorTree = ans ;
        ErrorMessage( 9 ) ;
      } // if
    
      if ( check->right->content == "nil" ) { // ( 等於nil代表他不可能有tf ) 
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
    while ( walk->right->content != "nil" ) { // 看有幾個argument 
      argument++ ;
      walk = walk->right ;
    } // while

    // 相當於if的條件句 ===============================================
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 看他是t or f 
      if ( ans->right->left->left->content == gDef_name[i].content ) { // /////
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for

    if ( ! isDef_exist && ! IsInt( ans->right->left->left->content ) && 
         ! IsFloat( ans->right->left->left->content ) &&
         ! IsString( ans->right->left->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
      temp = Which( ans->right->left->left ) ;
    } // if

    // ================================== 
    // 若是true 
    if ( temp->content == "#t" || ( temp->left != NULL && temp->right != NULL ) ||
         IsInt( temp->content ) || IsFloat( temp->content ) || IsString( temp->content ) ) { // 第一個輸出點 
      bool isNil = false ;
      walk = ans->right->left->right ;
      if ( walk->right->content == "nil" )
        isNil = true ;
      while ( walk->right->content != "nil" ) { // " 然後輸出正確的最後一個 " 錯要跳通知 
      
        if ( walk->left->left != NULL && walk->left->right != NULL ) {  
          gattempt = false ; // 這是non func error的判斷 
        } // if
        
        lasttemp = Which( walk->left ) ; 
        walk = walk->right ;
      } // while
    
      if ( isNil ) { // 若他一開始右邊就是nil 
        if ( walk->left->left != NULL && walk->left->right != NULL ) {
          gattempt = false ; // 這是non func error的判斷  
        } // if
        
        lasttemp = Which( walk->left ) ; // 那就直接做 
      } // if
    
      else
        lasttemp = Which( walk->left ) ;
    
      return lasttemp ;
    } // if
    
    // 第二條路 
    while ( ans->right->content != "nil" ) { // 第一條路沒輸出 那就到第二條路 
      ans = ans->right ; // 先往右邊走一格 
      argument-- ;
      if ( argument > 1 ) { // 大於一代表不是最後一條路 
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 第二條路 if 的判斷式 
          if ( ans->right->left->left->content == gDef_name[i].content ) {
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // 找到有變數被定義 
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->right->left->left->content ) && 
             ! IsFloat( ans->right->left->left->content ) &&
             ! IsString( ans->right->left->left->content ) ) { // 代表就是沒被定義過的sym 
          int num = ErrorNum( ans->right->left->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
          temp = Which( ans->right->left->left ) ;
      } // if

      // temp->content == "#t" || ( temp->left != NULL && temp->right != NULL ) ||
      //           IsInt( temp->content ) || IsFloat( temp->content ) || IsString( temp->content )
      if ( temp->content != "nil" && argument > 1 ) { // 第二條路true 
        bool isNil = false ;
        walk = ans->right->left->right ;
        if ( walk->right->content == "nil" ) // 跟上面相同 
          isNil = true ;
        while ( walk->right->content != "nil" ) { // 然後輸出最後一個 
        
          if ( walk->left->left != NULL && walk->left->right != NULL ) {
            gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
          } // if
      
          lasttemp = Which( walk->left ) ;
          walk = walk->right ;
        } // while
           
        if ( isNil ) {
          if ( walk->left->left != NULL && walk->left->right != NULL ) {
            gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
          } // if
        
          lasttemp = Which( walk->left ) ;
        } // if   
        
        else
          lasttemp = Which( walk->left ) ;
        
        return lasttemp ;
      } // if
      
      else if ( argument == 1 ) { // 印出default else ( 可能要在這邊做no return value )
        bool lastElse = false ;
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 第二條路 if 的判斷式 
          if ( ans->right->left->left->content == gDef_name[i].content ) {
            if ( ans->right->left->left->content == "else" )
              lastElse = true ;
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // 找到有變數被定義 
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->right->left->left->content ) && 
             ! IsFloat( ans->right->left->left->content ) &&
             ! IsString( ans->right->left->left->content ) ) { // 代表就是沒被定義過的sym 
          int num = ErrorNum( ans->right->left->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) // 沒找到 維持原樣繼續遞迴 
          temp = Which( ans->right->left->left ) ;
        
        if ( temp->content != "nil" || lastElse ) {
          bool isNil = false ; 
          walk = ans->right->left->right ;
          if ( walk->right->content == "nil" ) // 跟上面相同 
            isNil = true ;
              
          while ( walk->right->content != "nil" ) { // 應該要看它有幾個argument 然後輸出最後一個 
            if ( walk->left->left != NULL && walk->left->right != NULL ) {
              gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
            } // if
          
            temp = Which( walk->left ) ; // 先記錄著 有錯要直接跳錯 
            walk = walk->right ;
          } // while
            
          if ( isNil ) {
            if ( walk->left->left != NULL && walk->left->right != NULL ) {
              gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
            } // if
        
            temp = Which( walk->left ) ;
          } // if   
            
          else
            temp = Which( walk->left ) ;
              
          for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // 第二條路 if 的判斷式 
            if ( temp->content == gDef_name[i].content ) {
              temp = Which( gDef_name[i].point ) ;
            } // if
          } // for
              
          return temp ;
        } // if
    
        else {
          // 最後一個都錯誤那就是no return value 
          gErrorTree = re_temp ; // 給只有一行的cond ( no return value ) 
          ErrorMessage( 10 ) ;
        } // else
      } // else if
      
      isDef_exist = false ;
    } // while
    
    gErrorTree = re_temp ; // 給只有一行的cond ( no return value ) 
    ErrorMessage( 10 ) ;
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "begin" ) {
    if ( Num_argument( ans ) < 1 ) {
      throw new Exception7( "begin" ) ;
    } // if

    gattempt = true ; 
    Tree * temp = new Tree() ;
    Tree * re_temp = new Tree() ; // 最後要回傳的node 
    Tree * walk = new Tree() ;
    temp->pro = true ;
    re_temp->pro = true ;
    walk->pro = true ;
    temp->content = "" ;
    walk->content = "" ;
    walk = ans ;
    bool isDef_exist = false ; // 查看後面的tree 是否有已定義的變數 

    int argument = 0 ;
    while ( walk->right->content != "nil" ) { // 看有幾個argument 
      argument++ ;
      walk = walk->right ;
    } // while

    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
      if ( ans->right->left->content == gDef_name[i].content ) { // /////
        temp = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義 
      } // if
    } // for
    
    if ( ! isDef_exist && ! IsInt( ans->right->left->content ) && ! IsFloat( ans->right->left->content ) &&
         ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
      int num = ErrorNum( ans->right->left->content, NULL ) ;
      ErrorMessage( num ) ;
    } // if
    
    if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
      if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
        gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
      } // if

      temp = Which( ans->right->left ) ;
    } // if

    while ( ans->right->content != "nil" ) { // 多個argument 拿最後一個 
      ans = ans->right ;
      argument-- ;
      if ( argument > 1 ) {
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
          if ( ans->right->left->content == gDef_name[i].content ) {
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // 找到有變數被定義 
          } // if
        } // for
      
        if ( ! isDef_exist && ! IsInt( ans->right->left->content ) &&
             ! IsFloat( ans->right->left->content ) &&
             ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
          int num = ErrorNum( ans->right->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
      
        if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
          if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
            gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
          } // if

          temp = Which( ans->right->left ) ;
        } // if
      } // if
      
      if ( argument == 1 ) {
    
        for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
          if ( ans->right->left->content == gDef_name[i].content ) {
            temp = Which( gDef_name[i].point ) ;
            isDef_exist = true ; // 找到有變數被定義 
          } // if
        } // for
     
        if ( ! isDef_exist && ! IsInt( ans->right->left->content ) &&
             ! IsFloat( ans->right->left->content ) &&
             ! IsString( ans->right->left->content ) ) { // 代表就是沒被定義過的sym 
          int num = ErrorNum( ans->right->left->content, NULL ) ;
          ErrorMessage( num ) ;
        } // if
     
        if ( ! isDef_exist ) { // 沒找到 維持原樣繼續遞迴 
          if ( ans->right->left->left != NULL && ans->right->left->right != NULL ) {
            gattempt = false ; // cons的右的左可能是一顆 ( 1 2 3 ) 要做處理 
          } // if

          temp = Which( ans->right->left ) ;
        } // if

        return temp ;
      } // else if
      
      isDef_exist = false ;
    } // while
    
    return temp ; // 當只有一個argument 從這裡輸出 
  } // else if
  
  else if ( ans->left != NULL && ans->left->content == "let" ) {
    Tree * temp1 = new Tree() ; // x
    Tree * temp2 = new Tree() ; // y
    Tree * re_temp = new Tree() ; // 最後要回傳的node 
    temp1->pro = true ;
    temp2->pro = true ;
    re_temp->pro = true ;
    temp1->content = "" ;
    temp2->content = "" ;
    bool isDef_exist = false ; 
    for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
      if ( ans->right->left->left->left->content == gDef_name[i].content ) { // /////
        temp1 = Which( gDef_name[i].point ) ;
        isDef_exist = true ; // 找到有變數被定義
      } // if
    } // for

    if ( ! isDef_exist ) {
      temp1 = Which( ans->right->left->left->right->left ) ; // x
      gLocal_name.push_back( temp1 ) ;
    } // if
    
    isDef_exist = false ; 
    if ( ans->right->left->right->content != "nil" ) { // 有第二個local變數 
      
      for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
        if ( ans->right->left->right->left->content == gDef_name[i].content ) { //
          temp2 = Which( gDef_name[i].point ) ;
          isDef_exist = true ; // 找到有變數被定義
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
    
    if ( ans->right->left->content == "nil" ) { // 看有沒有參數 若沒有 
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
    
    else { // 暫且認定他只有一個參數 
    // Lambda( ans, ans->right->left->left->content ) ;
    } // else
    
    
    return temp1 ;
  } // else if 
  
  else if ( ans->content != "" ) { // 跟上下對調順序 
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
          for ( int i = 0 ; i < gDef_name.size() ; i++ ) { // if 的判斷式 
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

      if ( token.tokenlist.size() != 1 ) { // 單純想印define完的變數可以直接印 
        n_ans = Which( ans ) ;
        gIsNew_tree = true ;
      } // if

      if ( ! gIs_Define ) { // 沒要做define的動作才進 

        if ( n_ans->left != NULL && n_ans->right != NULL ) // token.tokenlist.size() != 1 origin
          cout << "( " ; // 防止 car '(1 2 3 ) 的結果 結果只有一個node 故不用印左括號 
        else
          gonly_one = true ;

        if ( token.tokenlist.size() != 1 ) // 因為要define 所以size不會只有1 
          Print( n_ans, space ) ;
        else { // 處理印 define好的樹 先用while找指標 
          while ( i < gDef_name.size() ) { // 找不到指標之後可能要印error 
            if ( ans->content == gDef_name[i].content ) {
              if ( gDef_name[i].point->left != NULL && gDef_name[i].point->right != NULL ) 
                cout << "( " ; // 代表那個指標的tree不只有一個node而已
              Print( gDef_name[i].point, space ) ;
              not_def_error = true ; // 有被定義過的話為true 
            } // if
        
            i++ ;
          } // while
          
          if ( ! not_def_error && ! IsInt( ans->content ) && ! IsFloat( ans->content ) && 
               ! IsString( ans->content ) && ans->content != "nil" && ans->content != "#t" &&
               ! ProFunction( ans ) ) {
            throw new Exception6( ans->content ) ; // 沒被定義過的變數要丟error ( unbounded ) 
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
    
    catch( Exception4 * e ) { // 遇到eof的跳出 
      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
      end = true ;
    } // catch()
    
    catch( Exception5 * e ) { // 看到 (exit) 跳出 
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
    
    catch( Exception7 * e ) { // 還要加東西 
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

    // 有問題 ( + 3 3 nil ) ) 會錯 
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
