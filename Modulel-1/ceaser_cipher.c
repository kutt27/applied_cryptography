#include <stdio.h>
#include <ctype.h>

void encrypt(char* msg, int shift){
	for(int i = 0; msg[i] != '\0'; i++){
		char ch = msg[i];
		if(isalpha(ch)){
			char base = isupper(ch) ? 'A' : 'a';
			// shift character within A-Z or a-z range
			msg[i] = (ch - base + shift) % 26 + base;
		}
	}
}

void decrypt(char* msg, int shift){
	for(int i = 0; msg[i] != '\0'; i++){
		char ch = msg[i];
		if(isalpha(ch)){
			char base = isupper(ch) ? 'A' : 'a';
			// shift character backwards, add 26 to avoid negative
			msg[i] = (ch - base - shift + 26) % 26 + base;
		}
	}
}

int main(){
	char message[1024];
	int shift;
	char choice;
	
	printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    printf("Enter shift key (1-25): ");
    scanf("%d", &shift);

    printf("Encrypt or Decrypt? (e/d): ");
    getchar();  // consume newline
    scanf("%c", &choice);
    
    if (choice == 'e' || choice == 'E') {
        encrypt(message, shift);
        printf("Encrypted message: %s\n", message);
    } else if (choice == 'd' || choice == 'D') {
        decrypt(message, shift);
        printf("Decrypted message: %s\n", message);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}

//    Modulel-1   git:(main)  ls
// ceaser_cipher.c  ceaser_cipher.md  README.md
//    Modulel-1   git:(main)  clang ceaser_cipher.c -o ceasor-cipher
//    Modulel-1   git:(main)  ./ceasor-cipher 
// Enter message: hello
// Enter shift key (1-25): 3
// Encrypt or Decrypt? (e/d): e
// Encrypted message: khoor

//    Modulel-1   git:(main)  ./ceasor-cipher
// Enter message: khoor
// Enter shift key (1-25): 3
// Encrypt or Decrypt? (e/d): d
// Decrypted message: hello
