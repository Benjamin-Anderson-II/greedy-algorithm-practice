#include <stdio.h>
#include <stdlib.h>

typedef struct {
	short int num;
	short int start;
	short int end;
} Act;

void printMaxActivities(Act *list, int size);
void mergeSort3(Act *data, int size);
void swap(Act *d, int p1, int p2);
void merge3(Act* data, Act *a1, int s1, Act *a2, int s2, Act *a3, int s3);
void _merge3helper(Act * arr1, int *index1, Act *arr2, int *index2, Act *data, int count);
int max3(int a, int b, int c);

int main(int argc, char const *argv[])
{
	FILE *fp;
	int numActivities;
	Act *actList;

	fp = fopen("act.txt", "r");
	for(int numSets = 1; fscanf(fp, "%d", &numActivities) != EOF; numSets++){

		//initialize actList
		actList = (Act *)malloc(numActivities * sizeof(Act));

		//scan in activities
		for(int i = 0; i < numActivities; i++)
			fscanf(fp, "%d %d %d", &actList[i].num, &actList[i].start, &actList[i].end);

		//sort in decending order of start times
		mergeSort3(actList, numActivities);

		printf("Set %d\n", numSets);
		printMaxActivities(actList, numActivities);
		free(actList);
	}

	return 0;
}

void printMaxActivities(Act *list, int size){
	int i = 0, j = 1, n = 1;

	int *print;
	print = (int *)malloc(size * sizeof(int));

	// since already sorted by decending start times, 
	// the first activity always gets selected
	print[0] = list[0].num;

	for(; j < size; j++){
		if(list[j].end <= list[i].start){
			print[n] = list[j].num;
			n++;
			i = j;
		}
	}

	printf("Maximum number of activities = %d\n", n);
	for(int k = n-1; k >= 0; k--)
		printf(" %d", print[k]);
	printf("\n\n");
}


/*******************
 * MERGE SORT HERE *
 *******************/
void mergeSort3(Act *data, int size) {
	int excess = size%3;
	int size1;
	int size2;
	int size3;
	Act *start1;
	Act *start2;
	Act *start3;

	switch(size){
	case 3:
		if(data[0].start < data[1].start) swap(data, 0, 1);
		if(data[1].start < data[2].start) swap(data, 1, 2);
		if(data[0].start < data[1].start) swap(data, 0, 1);
		break;
	case 2:
		if(data[0].start < data[1].start) swap(data, 0, 1);
		break;
	case 1:
		break;
	default:
		size1 = (excess> 0) ? size/3+1 : size/3;
		size2 = (excess==2) ? size/3+1 : size/3;
		size3 = size/3;

		start1 = data;
		start2 = data + size1;
		start3 = data + (size1 + size2);

		mergeSort3(start1, size1);
		mergeSort3(start2, size2);
		mergeSort3(start3, size3);

		merge3(data, start1, size1, start2, size2, start3, size3);

		break;
	}
}

void swap(Act *d, int p1, int p2) {
	Act temp = d[p1];
	d[p1] = d[p2];
	d[p2] = temp;
}

void merge3(Act* data, Act *a1, int s1, Act *a2, int s2, Act *a3, int s3){
	int c1 = 0;
	int c2 = 0;
	int c3 = 0;
	int max;
	Act temp[s1+s2+s3];

	while(c1 < s1 || c2 < s2 || c3 < s3){
		if(c1 == s1 && c2 == s2){
			temp[c1+c2+c3] = a3[c3];
			c3++;
		} else if(c2 == s2 && c3 == s3){
			temp[c1+c2+c3] = a1[c1];
			c1++;
		} else if(c3 == s3 && c1 == s1){
			temp[c1+c2+c3] = a2[c2];
			c2++;
		} else if(c1 == s1)
			_merge3helper(a2, &c2, a3, &c3, temp, c1+c2+c3);
		else if(c2 == s2)
			_merge3helper(a3, &c3, a1, &c1, temp, c1+c2+c3);
		else if(c3 == s3)
			_merge3helper(a1, &c1, a2, &c2, temp, c1+c2+c3); 
		else{
			max = max3(a1[c1].start, a2[c2].start, a3[c3].start);
			if(max == a1[c1].start){
				temp[c1+c2+c3] = a1[c1];
				c1++;
			} else if(max == a2[c2].start){
				temp[c1+c2+c3] = a2[c2];
				c2++;
			} else if(max == a3[c3].start){
				temp[c1+c2+c3] = a3[c3];
				c3++;
			}
		}
	}
	for(int i = 0; i < s1+s2+s3; i++)
		data[i] = temp[i];
}

void _merge3helper(Act * arr1, int *index1, Act *arr2, int *index2, Act *data, int count) {
	if(arr1[*index1].start > arr2[*index2].start){
		data[count] = arr1[*index1];
		*index1 = *index1 + 1;
	} else {
		data[count] = arr2[*index2];
		*index2 = *index2 + 1;
	}
}

int max3(int a, int b, int c) {
	if(a >= b && a >= c) return a;
	if(b >= a && b >= c) return b;
	return c;
}