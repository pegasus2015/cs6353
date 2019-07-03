#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/ssl.h>

/* function definitions */
void getNumBytes(int *num_bytes);
void getByteValues(int *nums, int num_bytes);
void buildRandomString(unsigned char *str);
int  createHash(const char *message, unsigned char *md_value, int *md_len);
void printMessage(const unsigned char* message, const int len);
void printHash(const unsigned char *hashstring, const int md_len);
int  searchHash(const unsigned char *hash, int len, const int *prefix, int plen);
void printPrefix(const int *prefix, int numbytes);
void checkArrays(const unsigned char *hash, int len, const int *prefix, int numbytes);

int main(int argc, char *argv[]){
   
   // SEED the random num generator
   srand((unsigned)time(NULL));
   
   const int MSG_LEN = 20;
   
   // Result of Hash search   
   //int res;
   int notfound;
   double runs;

   // User input vars
   int num_bytes; // number of bytes to match hash digest
   int c; // char from user input  
   
   // Message array
   unsigned char message[MSG_LEN];    
   
   // Hash variables
   unsigned char md_value[EVP_MAX_MD_SIZE]; // hash array
   int md_len; // hash length

   getNumBytes(&num_bytes);
 
   // Array to store byte values
   int prefix[num_bytes]; 
   
   getByteValues(prefix, num_bytes);
   
   printf("TargetDigestPrefix in hexadecimal\n"); 
   printPrefix(prefix, num_bytes);
   printf("\n");
   
   notfound = 1; // set flag variable for loop
   runs = 0; 
   
   do {
      // Create a message
      buildRandomString(message);
   
      // Hash the message
      createHash(message, md_value, &md_len);   
    
      // Perform a search
      notfound = searchHash(md_value, md_len, prefix, num_bytes);   
                
      runs++;
   }
   while(notfound);
   
   // Report results
   printf("Match in %.6f runs\n", runs);
   printf("Message in hexadecimal format\n");
   printMessage(message, MSG_LEN);
   printf("MD5 Message Digest in hexadecimal format\n"); 
   printHash(md_value, md_len);   
   printf("\n");
   return 0;
} // end of main

int searchHash(const unsigned char *hash, int len, const int *prefix, int numbytes){
   int  i, count = 0;
   for (i = 0; i < numbytes; i++){
      if (prefix[i] != hash[i]){
         break;
      }
      else{
         count++; // user's byte value matches a digest value 
      }   
   }
   if (numbytes == count){
      return 0; // prefix matches digest, notfound is false
   }
   return 1; // no match, notfound is true. 
   
} // end of searchHash function

void getNumBytes(int *num_bytes){
   
   int c; // char from user input
   
   // Get number of bytes to match 
   printf("Enter Number of Bytes to Match\n");

   // Get user input for number of bytes
   scanf("%d", num_bytes);

   // Get whitespace char
   c = getchar();    
} // end of getNumBytes function

void getByteValues(int *nums, int num_bytes){
   int c;
   
   printf("Enter the byte values to match for %d bytes\n", num_bytes);
   
   for (int i = 0; i < num_bytes; i++){
      scanf("%d", &c);
      nums[i] = c;
      c = getchar(); // get whitespace char      
   }
} // end of getByteValues function

void buildRandomString(unsigned char * str){
   unsigned char c; 
   int i;
   for (i = 0 ; i < 20; i++){
     c = rand() % 256; 
     str[i] = c;
   }
   str[20] = '\0';
} // end of buildRandomString function

int createHash(const char* message, unsigned char* md_value, int* md_len){
   EVP_MD_CTX *mdctx;
   unsigned char **digest;
   unsigned int *digest_len; // move to main
   size_t message_len; 
      
   if ((mdctx = EVP_MD_CTX_create()) == NULL){
      printf("Error init in CTX.");
      return -1;   
   }
   if ( EVP_DigestInit_ex(mdctx, EVP_md5(), NULL)  != 1){
      printf("Error Digest Init!");
      return -1;
   }   
   if (EVP_DigestUpdate(mdctx, message, strlen(message)) != 1){
      printf("Error in Digest Update!");
      return -1;
   }
   EVP_DigestFinal_ex(mdctx, md_value, md_len); 
   EVP_MD_CTX_destroy(mdctx);

} // end of createHash function


void printMessage(const unsigned char* message, int len){
   int i;
   for (i = 0; i < len; i++){
      printf("%02x", message[i]);
   }
   printf("\n");
} // end of printMessage function

void printHash(const unsigned char* hashstring, const int md_len){
   int i;
   for (i = 0; i < md_len; i++){
      printf("%02x", hashstring[i]);
   }
   printf("\n");
} // end of printHash function

void printPrefix(const int *prefix, int numbytes){
   int i;   
   for (i = 0; i < numbytes; i++){
      printf("%02x", prefix[i]);
   }
   printf("\n");   
} // end of printPrefix function

void checkArrays(const unsigned char *hash, int len, const int *prefix, int numbytes){
   int i;
   printf("Searching hash... \n");
   for (i = 0; i < len; i++){
      printf("%02x ", hash[i]);
   }
   printf("\n");
   printf("Looking at prefix...\n");
   for (i = 0; i < numbytes; i++){
      printf("%02x ", prefix[i]);
   }   
} // end of checkArrays function
