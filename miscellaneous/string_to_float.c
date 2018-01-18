# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int main(){
    float val;
    char str[5] = "12.04";

    val = atof(str);

    printf("Value: %f%", val);
    return 0;
}
