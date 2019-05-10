#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	//generate ASCII code from 32(space) to 126(~) and return it as a character
    return rand() % (126 - 32) + 32;
}

char *inputString()
{
	int length = 6;
	//generate a random character the length of the string
	int i;
	char tempChar;
	char* ranString = malloc(sizeof(char) * (length + 1));
	memset(ranString, '\0', sizeof(ranString));
	for (i = 0; i < length-1; i++) {
		char temp = rand() % (117-101) + 101;
		ranString[i] = temp;
	}
	ranString[length] = '\0';	//add null terminator to end of string
	return ranString;			//return random, null-terminated string
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
    free(s);
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
