#include <stdio.h>


int NibbleSub[16]={0xE,0x4,0xD,0x1,0x2,0xF,0xB,0x8,0x3,0xA,0x6,0xC,0x5,0x9,0x0,0x7};
int NibbleSub_Inverse[16]={0xE,0x3,0x4,0x8,0x1,0xC,0xA,0xF,0x7,0xD,0x9,0x6,0xB,0x2,0x0,0x5};
unsigned int key = 0xc3f0;
unsigned int input = 0x9c63;

int Length(unsigned int n){
    int c = 0 ; // counter
    while (n)
    {
        c++ ;
        n >>= 1 ;
    }
    return c ;
} 



unsigned int pol_multi(unsigned int input_pol1,unsigned int input_pol2) {

    unsigned int result=0b0000;
    unsigned int buffer1;
    unsigned int divided=0b10011;
    int n=4,m;
    for(int i=0; i<n; i++) {
        m= input_pol2 & 1;
        input_pol2 >>= 1;        
        if(m){
            buffer1 = input_pol1;
            buffer1 <<= i;
            result = result^buffer1;
            }

    }
    
    // buffer2=Length(result);
    // buffer3=Length(divided);
    while(Length(result)>=Length(divided)){
            
        result = result^(divided << (Length(result)-Length(divided)));        
    }
    
    
    return result;

    
}

unsigned int bit_16_to_4(unsigned int input_16bit,unsigned int output_matrix[2][2]){
    int buffer2,buffer3;
    // buffer2= input_16bit;
    // buffer3= input_16bit >>8;
    for(int i= 0; i<2; i++){
        buffer2= input_16bit;
        buffer2 >>= 4*i;
        output_matrix[1-i][1] = buffer2 & 0x000F;
    }

    for(int j=0; j<2; j++){
        // buffer3= input_16bit >> 8;
        buffer3 = input_16bit >> 4*(j+2);
        output_matrix[1-j][0] = buffer3 & 0x000F;
    }

    return output_matrix;
}



unsigned int SubNibble(unsigned int input_Nibble[2][2],unsigned int output_Nibble[2][2]){

    // int NibbleSub[16]={0xE,0x4,0xD,0x1,0x2,0xF,0xB,0x8,0x3,0xA,0x6,0xC,0x5,0x9,0x0,0x7};
	for(int i=0;i<2;i++)
	{	
		for(int j=0; j<2;j++){
            output_Nibble[i][j]= NibbleSub[input_Nibble[i][j]];
        }
	}
	return output_Nibble ;
}

unsigned int Inverse_Nibble(unsigned int input_Nibble[2][2],unsigned int output_Nibble[2][2]){

    // int NibbleSub_Inverse[16]={0xE,0x3,0x4,0x8,0x1,0xC,0xA,0xF,0x7,0xD,0x9,0x6,0xB,0x2,0x0,0x5};
	for(int i=0;i<2;i++)
	{	
		for(int j=0; j<2;j++){
            output_Nibble[i][j]= NibbleSub_Inverse[input_Nibble[i][j]];
        }
	}
	return output_Nibble ;
}


unsigned int Shiftrow(unsigned int input_matrix[2][2], unsigned int output_matrix[2][2]){

    output_matrix[0][0]=input_matrix[0][0];
    output_matrix[0][1]=input_matrix[0][1];
    output_matrix[1][0]=input_matrix[1][1];
    output_matrix[1][1]=input_matrix[1][0];
    return output_matrix;
}

unsigned int Mixcolumn(unsigned int input_matrix[2][2], unsigned int output_matrix[2][2]){
    unsigned int constant_matrix[2][2] = {0b0011, 0b0010, 0b0010, 0b0011};
    output_matrix[0][0]=pol_multi(input_matrix[0][0],constant_matrix[0][0]) ^ pol_multi(input_matrix[1][0],constant_matrix[0][1]);
    output_matrix[1][0]=pol_multi(input_matrix[0][0],constant_matrix[1][0]) ^ pol_multi(input_matrix[1][0],constant_matrix[1][1]);
    output_matrix[0][1]=pol_multi(input_matrix[0][1],constant_matrix[0][0]) ^ pol_multi(input_matrix[1][1],constant_matrix[0][1]);
    output_matrix[1][1]=pol_multi(input_matrix[0][1],constant_matrix[1][0]) ^ pol_multi(input_matrix[1][1],constant_matrix[1][1]);
    return output_matrix;
}

unsigned int KeyAddition(unsigned int input_block[2][2],unsigned int Ki[2][2], unsigned int output_block[2][2]){
    
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            output_block[i][j] = input_block[i][j] ^  Ki[i][j];
        }
    }
    return output_block;
}

unsigned int KeySchedule(unsigned int K_0[2][2], unsigned int K_1[2][2], unsigned int K_2[2][2]){
    unsigned int w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11;//round keys
    w0 = K_0[0][0];
    w1 = K_0[1][0];
    w2 = K_0[0][1];
    w3 = K_0[1][1];
    w4 = w0 ^ NibbleSub[w3] ^ 0b0001;
    w5 = w4 ^ w1;
    w6 = w5 ^ w2;
    w7 = w6 ^ w3;
    w8 = w4 ^ NibbleSub[w7] ^ 0b0010;
    w9 = w8 ^ w5;
    w10 = w9 ^ w6;
    w11 = w10 ^ w7;
    K_1[0][0]=w4;
    K_1[1][0]=w5;
    K_1[0][1]=w6;
    K_1[1][1]=w7;
    K_2[0][0]=w8;
    K_2[1][0]=w9;
    K_2[0][1]=w10;
    K_2[1][1]=w11;
    return K_1, K_2;
}

unsigned int Nibbles_to_bit16(unsigned int Nibbles_Matrix[2][2], unsigned int bit16_out){
    unsigned int buffer4, buffer5, buffer6, buffer7;
    buffer4 = Nibbles_Matrix[0][0];
    buffer5 = Nibbles_Matrix[1][0];
    buffer6 = Nibbles_Matrix[0][1];
    buffer7 = Nibbles_Matrix[1][1];

    buffer4 <<= 4;
    buffer6 <<= 4;
    buffer4 = buffer4 | buffer5;
    buffer6 = buffer6 | buffer7;

    buffer4 <<= 8;
    bit16_out = buffer4 | buffer6 ;

    return bit16_out;
}


unsigned int Encryption(unsigned int bit16_input, unsigned int bit16_output, unsigned int K0[2][2], unsigned int K1[2][2], unsigned int K2[2][2]){
    unsigned int P0_Matrix[2][2]={}, P1_Matrix[2][2]={}, P2_Matrix[2][2]={}, P3_Matrix[2][2]={}, P4_Matrix[2][2]={}, P5_Matrix[2][2]={},
    P6_Matrix[2][2]={}, P7_Matrix[2][2]={}, P8_Matrix[2][2]={}; 
    unsigned int Nibble1, Nibble2, Nibble3, Nibble4;//text nibbles

    bit_16_to_4(bit16_input,P0_Matrix);

    Nibble1=P0_Matrix[0][0];
    Nibble2=P0_Matrix[1][0];
    Nibble3=P0_Matrix[0][1];
    Nibble4=P0_Matrix[1][1];
    printf("%x %x %x %x \n", Nibble1,Nibble2,Nibble3,Nibble4);
    
    KeyAddition(P0_Matrix, K0, P1_Matrix);// round 0
    SubNibble(P1_Matrix, P2_Matrix);
    Shiftrow(P2_Matrix, P3_Matrix);
    Mixcolumn(P3_Matrix, P4_Matrix);
    KeyAddition(P4_Matrix, K1, P5_Matrix);// round 1
    SubNibble(P5_Matrix, P6_Matrix);
    Shiftrow(P6_Matrix, P7_Matrix);
    KeyAddition(P7_Matrix, K2, P8_Matrix);// round 2

    Nibble1=P4_Matrix[0][0];
    Nibble2=P4_Matrix[1][0];
    Nibble3=P4_Matrix[0][1];
    Nibble4=P4_Matrix[1][1];
    printf("%x %x %x %x \n", Nibble1,Nibble2,Nibble3,Nibble4);

    bit16_output=Nibbles_to_bit16(P8_Matrix, bit16_output);

    return bit16_output;

}

unsigned int Decryption(unsigned int encode_input, unsigned int decode_output, unsigned int K0[2][2], unsigned int K1[2][2], unsigned int K2[2][2]){
    unsigned int P0_Matrix[2][2]={}, P1_Matrix[2][2]={}, P2_Matrix[2][2]={}, P3_Matrix[2][2]={}, P4_Matrix[2][2]={}, P5_Matrix[2][2]={},
    P6_Matrix[2][2]={}, P7_Matrix[2][2]={}, P8_Matrix[2][2]={}; 
    unsigned int Nibble1, Nibble2, Nibble3, Nibble4;//text nibbles

    bit_16_to_4(encode_input,P0_Matrix);

    Nibble1=P0_Matrix[0][0];
    Nibble2=P0_Matrix[1][0];
    Nibble3=P0_Matrix[0][1];
    Nibble4=P0_Matrix[1][1];
    printf("%x %x %x %x \n", Nibble1,Nibble2,Nibble3,Nibble4);
    
    KeyAddition(P0_Matrix, K2, P1_Matrix);
    Shiftrow(P1_Matrix, P2_Matrix);
    Inverse_Nibble(P2_Matrix, P3_Matrix);
    KeyAddition(P3_Matrix, K1, P4_Matrix);
    Mixcolumn(P4_Matrix, P5_Matrix);
    Shiftrow(P5_Matrix, P6_Matrix);
    Inverse_Nibble(P6_Matrix, P7_Matrix);   
    KeyAddition(P7_Matrix, K0, P8_Matrix);
    
    Nibble1=P4_Matrix[0][0];
    Nibble2=P4_Matrix[1][0];
    Nibble3=P4_Matrix[0][1];
    Nibble4=P4_Matrix[1][1];
    printf("%x %x %x %x \n", Nibble1,Nibble2,Nibble3,Nibble4);

    decode_output=Nibbles_to_bit16(P8_Matrix, decode_output);

    return decode_output;
}



int main() {
    unsigned int encoded_output, decoded_output;
    unsigned int K0[2][2]={}, K1[2][2]={}, K2[2][2]={};
    bit_16_to_4(key, K0);
    KeySchedule(K0, K1, K2);

    printf("plaintext is %#x  \n", input);
    encoded_output = Encryption(input, encoded_output, K0, K1, K2);
    printf("encoded text is %#x  \n", encoded_output);
    decoded_output = Decryption(encoded_output, decoded_output, K0, K1, K2);
    printf("decoded text is %#x  \n", decoded_output);



return 0;




// unsigned int Nibble1 = SubNibble(input);




// unsigned int Nibble_0[4]={Nibble1,Nibble2,Nibble3,Nibble4};


// unsigned int k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11;
// k0=K0[0][0];
// k1=K0[1][0];
// k2=K0[0][1];
// k3=K0[1][1];
// k4=K1[0][0];
// k5=K1[1][0];
// k6=K1[0][1];
// k7=K1[1][1];
// k8=K2[0][0];
// k9=K2[1][0];
// k10=K2[0][1];
// k11=K2[1][1];

// printf("%x %x %x %x %x %x %x %x %x %x %x %x", k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11);
// unsigned int pol1= 0b1001;
// unsigned int pol2= 0b0111;
// unsigned int l=pol_multi(pol1 ,pol2);



}
