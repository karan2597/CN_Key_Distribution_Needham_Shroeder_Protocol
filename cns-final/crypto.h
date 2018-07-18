void encrypt(int key, char * arr)
{	int i;
	for(i=0;arr[i]!='\0';i++)
		arr[i]=arr[i]+key;
}

void decrypt(int key, char * arr)
{	int i;
	for(i=0;arr[i]!='\0';i++)
		arr[i]=arr[i]-key;
}
