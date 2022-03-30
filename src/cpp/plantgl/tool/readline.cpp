/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */
				

#include "readline.h"

#ifdef USE_READLINE

#include <iostream>
#include <stdlib.h>
#include <assert.h>
/*
#define ON_ERROR failwith(our_error_message());
*/

static const char* DEFAULT_PROMPT = ">";

#if 0
#define NOHISTORY
#endif

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

// int rl_blink_matching_paren = 1;

typedef const char* mt;
mt * key_word(NULL);
/*[]={       
  "VOID" ,
  (char *)NULL      
};*/                    

static int list_index;


/*! Function that detects if a given word is a key_word from the table key_word[].
 If the key_word is in the table, the returned value is a copy of the key_word argument (text).
 It is a char* that must be freed by the callee.
 If the function is called a second time, a second occurence of the key_word is searched for.
 To reinitialise the search, argument state must be set to 0

*/

const char *keyword_generator(const char* text, int state) {

    static int len;

    const char *key;

//    cerr << "Text : " << text << ", State : " << state << " , index : " << list_index << endl;
    if (!state /*&& !list_index*/)
    {
	list_index = 0;
	len = strlen(text);
    }

    if(key_word){
      while((key = key_word[list_index])) // stops when the end of the array is reached :(char *)NULL
	{
	  list_index++;
	  
	  /** !!! Attention, faut dupliquer le mot car il sera libere plus tard !!! ***/
	  if (strncmp(key, text, len) == 0)
	    return(strdup(key));	
	}
    }

    return ((const char *)NULL);

}

const char **keyword_completion(const char* text, int start, int end) {

    const char **matches;
    // char *keyword_generator();

    matches=(const char **)NULL;

//    cerr << "Text : " << text << ", start " << start  << ", end " << end <<  endl;
    if(start == 0)
#if defined (_RL_FUNCTION_TYPEDEF)
      matches=(const char **)rl_completion_matches(text,(rl_compentry_func_t*)keyword_generator); 
#else
      matches=(const char **)completion_matches(text,(CPFunction*)keyword_generator);
#endif

    return(matches);
}



void setKeyword(const char ** keyword)
{
  key_word = keyword;
}

void gnu_init_readline()
{
  using_history();
#if defined (_RL_FUNCTION_TYPEDEF)
  rl_attempted_completion_function =  (rl_completion_func_t*) keyword_completion;
#else
  rl_attempted_completion_function =  (CPPFunction*) keyword_completion;
#endif
  /*    rl_completion_entry_function     =  keyword_completion1; */
}


int readline_input(char* buff, const char* prompt) {

  static char *ligne=(char *)NULL;
  int done=0;

  if (ligne) free(ligne);
  ligne=(char *)readline(prompt? // readline does not return the \n
			 (strlen(prompt)?(char*)prompt:DEFAULT_PROMPT): 
			 DEFAULT_PROMPT); 
  if (!ligne) done=1;

  if (done) return (0);
  if (*ligne) /* le premier caractere est-il nul ? */
  {

    register char* p = ligne;
    register char* ptr = buff;
    register int lg = 0;

#ifndef NOHISTORY
    add_history(ligne);
#endif

    while ((*ptr++ = *p++)) lg++; // copy of ligne into buff 

    --ptr;
    *ptr++ = '\n';		// put "\n\0" at the end of the string
    *ptr++ = '\0';
      
    // cerr << "readline: line lg = " << lg+1 << endl;

    return(lg+1);
    
  }
  else { // if readline returns 
    buff[0] = '\n';
    buff[1] = '\0';

    // cerr << "readline: line null, lg = 1" << endl;

    return 1;
  }
}

#endif


