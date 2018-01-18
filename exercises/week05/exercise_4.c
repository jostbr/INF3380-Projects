
# include <stdio.h>

void message_interaction(int i){
	int messages_sent[5] = {0, 0, 1, 0, 1};
	int i0_received[3] = {1,2,4};
	int i1_received[2] = {3,5};
	int j;

	printf("i = %d:\n", i);

	if (i == 0){
		for (j = 0; j < sizeof(i0_received)/sizeof(int); j++){
			printf("Received from %d\n", i0_received[j]);
		}
		
	}

	else if (i == 1){
		for (j = 0; j < sizeof(i1_received)/sizeof(int); j++){
			printf("Received from %d\n", i1_received[j]);
		}
	}

	if (i > 0){
		printf("Sent to %d\n", messages_sent[i]);	
	}

	
}

int main(){
	message_interaction(5);
}