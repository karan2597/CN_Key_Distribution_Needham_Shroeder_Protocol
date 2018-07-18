#include<stdio.h>
#include<stdlib.h>
int main()
{	int a=83;
	char c[10];
	sprintf(c,"%d",a);
	printf("%c",c[0]);

	char b=c[0];
	printf("%c%c",b,b+5);
	a=atoi(b+5)	
	return 0;
}
