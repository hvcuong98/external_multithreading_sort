#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;


void sortBlock(vector<string> v, int nBlock)
{
    char fileName[12];
    sprintf(fileName, "%d", nBlock);
    FILE *ou = fopen(fileName, "w");
    sort(v.begin(), v.end());
    for(int i = 0; i < v.size(); i++)
    {
        for(int j = 0; j < v[i].size(); j++)
            fputc(v[i][j], ou);
        fputc('\n', ou);
    }
    fclose(ou);
}

int main(int argc, char *argv[])
{
    char *inputFile = argv[1]; //Input file name
    char *outputFile = argv[2]; // Output file name
    long long memoryLimit = atoll(argv[3]); //Memory limit in bytes
    memoryLimit -= 100;

    //Devide input file into small files that fit in memory limit
    char c;
    string s;
    int nBlock = 0;
    vector<string> v;
    long long temp = 0;
    thread taskSort;
    bool isCalled = false;
    FILE *inputStream = fopen(inputFile, "r");
    while (1)
    {
        c = fgetc(inputStream);
        if (c == EOF) break;
        if (c == '\n')
        {
            v.push_back(s);
            s.clear();
        } else
        {
            temp++;
            s += c;
            if (temp == memoryLimit / 4)
            {
                if (isCalled) taskSort.join();
                taskSort = thread(sortBlock, v, nBlock++);
                isCalled = true;
                v.clear();
                temp = s.size();
            }
        }
    }
    if (!s.empty())
    {
        v.push_back(s);
        s.clear();
    }
    if (!v.empty())
    {
        if (isCalled) taskSort.join();
        isCalled = true;
        taskSort = thread(sortBlock, v, nBlock++);
        v.clear();
    }
    if (isCalled) taskSort.join();
    fclose(inputStream);

    //Merge these sorted small files and print to output file
    FILE *in[nBlock];
    for(int i = 0; i < nBlock; i++)
    {
        char fileName[12];
        sprintf(fileName, "%d", i);
        in[i] = fopen(fileName, "r");
    }
    set<pair<string, int> > minSet; //create a set with nBlock nodes, compare by first element in each block
    int cnt = 0;
    for(int i = 0; i < nBlock; i++)
        if ((c = fgetc(in[i])) != EOF)
        {
            s += c;
            cnt++;
            while (1)
            {
                c = fgetc(in[i]);
                if (c == '\n' || c == EOF) break;
                s += c;
            }
            minSet.insert({s, i});
            s.clear();
        }
    FILE *outputStream = fopen(outputFile, "w");
    while (cnt > 0)
    {
        s = minSet.begin()->first;
        for(int i = 0; i < s.size(); i++)
            fputc(s[i], outputStream);
        fputc('\n', outputStream);
        int i = minSet.begin()->second;
        s.clear();
        minSet.erase(minSet.begin());
        if ((c = fgetc(in[i])) == EOF) cnt--; else
        {
            s += c;
            while (1)
            {
                c = fgetc(in[i]);
                if (c == '\n' || c == EOF) break;
                s += c;
            }
            minSet.insert({s, i});
            s.clear();
        }
    }

    //Close all stream
    for(int i = 0; i < nBlock; i++)
        fclose(in[i]);
    fclose(outputStream);
    return 0;
}


