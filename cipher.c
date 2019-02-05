#include <stdio.h>
//Array to transpose bits in an input block
int subst[16]={2,15,3,14,1,13,10,4,6,0,5,8,11,7,12,9}; 

//ECB Encryption Algorithm
void *encryptECB(const char *file_name){

        //file for reading image
        FILE *file_input;
        file_input = fopen(file_name, "rb");

        //file for writing encrypted image
        FILE *file_output;
        file_output = fopen("ecb_encrypted_image.ppm", "wb");

        //block allocation
        char block[16];

        //encrypted block
        char ecb_block[16];

        int counter=0;
        char bit;
        int block_index;
        int image_index;
        
    while(counter<54){ 
        //Get header bits from input file to the output file
        bit = fgetc(file_input); 
        fprintf(file_output,"%c",bit); 
        counter=counter+1 ;
    }

    while(counter>53){
        // Get image bits after header
        if(feof(file_input)){ // If end of file return
            break;
        }

        bit = fgetc(file_input);
        image_index=counter-54; // get image bit position without header
        block_index=image_index%16; //get corresponding bit position of a block
        block[block_index]=bit; //create a 16 bit block

        //When 16bit block is complete map image bits into new encrypted block
        if (block_index==15){
            for (int i=0;i<16;i++){
                ecb_block[subst[i]]=block[i]; 
            }
            for (int i=0;i<16;i++){ //insert the bits into the new encrypted file
                fprintf(file_output,"%c",ecb_block[i]); 
            }
        } 

        counter=counter+1;

    }

    fclose(file_input);
    fclose(file_output);
    return 0;

}

//CBC Encryption Algorithm
void *encryptCBC(const char *file_name){

    //file for reading image
    FILE *file_input;
    file_input = fopen(file_name, "rb");

    //file for writing encrypted image
    FILE *file_output;
    file_output = fopen("cbc_encrypted_image.ppm", "wb");

    //block allocation
    char block[16];

    //encrypted block
    char cbc_block[16];

    int counter=0;
    char bit;
    int block_index;
    int image_index;

    char previous_block[16];
    char xored_block[16];

    for(int i=0;i<16;i++){
        // set initial peviously encrypted block values to 0
        previous_block[i]=0;
    }

    while(counter<54){ 
        // get header bits from input file to the output file
        bit = fgetc(file_input); 
        fprintf(file_output,"%c",bit); 
        counter=counter+1 ;
    }

    while(counter>53){
        // get image bits after header
        if(feof(file_input)){ // if end of file break loop
            break;
        }

        bit = fgetc(file_input);
        image_index=counter-54; // get image bit position without header
        block_index=image_index%16; //get corresponding bit position of a block
        block[block_index]=bit; //create a 16 bit block

        if (block_index==15){

            //xor the bits with previous encrypted block
            for (int i=0;i<16;i++){
                xored_block[i]=block[i]^previous_block[i];
            }

            //perform transposition with previos block
            for (int i=0;i<16;i++){
                cbc_block[subst[i]]=xored_block[i]; 
            }
             
            for (int i=0;i<16;i++){
                //replace bits in previously encrypted block with newly encrypted block bits
                previous_block[i] = cbc_block[i];  
                fprintf(file_output,"%c",cbc_block[i]); 
            }   
        }

        counter=counter+1;

    }
    fclose(file_input);
    fclose(file_output);
    return 0;
}

int main(){
    encryptECB("watercolor.ppm");
    encryptCBC("watercolor.ppm");
    return 0;
}