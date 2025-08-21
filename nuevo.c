#include <stdio.h>

int main(void) {
    int a,b,c;
    scanf("%d",&a);
    for (b=2;a>=b;b++){
	int cont=0;
	if(b==2){
	    printf("%d \t",b);
	}
	else{
	
	for (c=2;c<b;c++){
	    if (b%c==0){
		cont=1;
		break;
	}
	}
	if(cont==0){
	    printf("%d \t",b);
	    }
	}
    }
    return 0;
    }
