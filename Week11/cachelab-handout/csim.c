// 2018030328
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

typedef unsigned long long int memAddr;

typedef struct{
    short valid;
    memAddr tag;
    int lastUse; // time stamp
}_Line;

typedef struct{
    _Line *lines;
}_Set;

typedef struct{
    _Set *sets;
}_Cache;

// optional
int v; // show status in cache

// parameters
int s = 0;
int b = 0;
int S = 0; // 2^s -> # of sets/cache
int B = 0; // 2^b -> # of bytes/block
int E = 0; // # of lines/set
char *T;   // file name

// count
int hit = 0;
int miss = 0;
int eviction = 0;

int cnt = 0;

void printInfo(char* argv[]);
_Cache initCache(void);
void searchInCache(_Cache c, memAddr addr);
_Cache updateCache(_Cache cache, memAddr Tag, memAddr Set);
int findVictim(_Cache cache, memAddr Set);
long long int Pow(long long int base, unsigned int exp);
void printSummary(int hits, int misses, int evictions);

int main(int argc, char *argv[]){
    int option;    
    FILE *fp;
    char op;
    int bytes;
    memAddr addr;
    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1){
        switch (option) {
			case 'h':
                printInfo(argv);
				break;
            case 'v':
                v = 1;
                break;
            case 's':
                s = atoi(optarg);
                S = Pow(2.0, s);
                break;            
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                B = Pow(2.0, b);
                break;
            case 't':
                T = optarg;
                break;
            default:
                break;
        }
    }  

    if(S == 0 || E == 0 || B == 0 || T == NULL){
        printf("Error: 필수 Option을 입력하세요.\n");
        return 0;
    }
    
    _Cache cache = initCache();
    fp = fopen(T, "r");
    if(fp != NULL){
        while(fscanf(fp, " %c %llx,%d", &op, &addr, &bytes) == 3){
			if(v) printf("%c %llx,%d", op, addr, bytes);
            switch (op) {
                case 'L':
                    searchInCache(cache, addr);
                    cnt++;
                    break;
                case 'S':
                    searchInCache(cache, addr);
                    cnt++;
                    break;
                case 'M':
                    searchInCache(cache, addr);
                    cnt++;
                    searchInCache(cache, addr);
                    cnt++;
                    break;
                default:
                    break;
            }
			if(v) printf("\n");
        }
    }
    
    printSummary(hit,miss,eviction);
    fclose(fp);
    
    return 0;
}

void printInfo(char* argv[]){
    printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
    printf("Flags:\n");
    printf("  -h    각 플래그 사용 방법에 대한 도움말 출력(optional)\n");
    printf("  -v    trace 파일에서 각 메모리 접근에 대한 캐시 동작 결과 출력 (optional)\n");
    printf("  -s    캐시 set에 대한 index bits 수 (2^s = # of sets/cache)\n");
    printf("  -E    Associativity (E = # of lines/sets)\n");
    printf("  -b    캐시 블록에 대한 bits 수 (2^b = # of bytes/block)\n");
    printf("  -t    시뮬레이션을 진행할 메모리 trace 파일을 지정\n");
}

long long int Pow(long long int base, unsigned int exp){
    if(exp > 0)
	    return base * Pow(base, exp-1);
    return 1;
}

_Cache initCache(void){
    _Cache cache;
    _Set set;
    _Line line;
    
    cache.sets = (_Set*)malloc(sizeof(_Set)*S);
    
    for(int i = 0; i < S; i++){
        set.lines = (_Line*)malloc(sizeof(_Line)*E);
        for (int j = 0; j < E; j++) {
            line.valid = 0;
            line.tag = 0;
            line.lastUse = 0;
            set.lines[j] = line;
        }
        cache.sets[i] = set;
    }
    
    return cache;
}

void searchInCache(_Cache c, memAddr addr){
    memAddr keySet = addr << (64 - (s + b)) >> (64 - (s));
    memAddr keyTag = addr >> (s + b);
    int i = 0;
    for (i = 0; i < E; i++){
        // cache hit
        if(c.sets[keySet].lines[i].valid != 0 && c.sets[keySet].lines[i].tag == keyTag){
            hit++;
			if(v) printf(" hit");
            c.sets[keySet].lines[i].lastUse = cnt;
            return;
        }
    }
    // cache miss
	miss++;
	if(v) printf(" miss");
    c = updateCache(c, keyTag, keySet);
}

_Cache updateCache(_Cache cache, memAddr Tag, memAddr Set){
    for (int i = 0; i < E; i++){
        if(cache.sets[Set].lines[i].valid == 0){
            cache.sets[Set].lines[i].valid = 1;
            cache.sets[Set].lines[i].tag = Tag;
            cache.sets[Set].lines[i].lastUse = cnt;
            return cache;
        }
    }
	eviction++;
	if(v) printf(" eviction");
    int replaceIndex = findVictim(cache, Set);
    
    cache.sets[Set].lines[replaceIndex].valid = 1;
    cache.sets[Set].lines[replaceIndex].tag = Tag;
    cache.sets[Set].lines[replaceIndex].lastUse = cnt;
    
    return cache;
}

int findVictim(_Cache cache, memAddr Set){
    int minIndex = 0;
    for (int i = 1; i < E; i++){
        if(cache.sets[Set].lines[i].lastUse < cache.sets[Set].lines[minIndex].lastUse){
            minIndex = i;
        }
    }
    return minIndex;
}

void printSummary(int hits, int misses, int evictions){
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}
