#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

// O clasa utila pentru a modela argumentele Mapper-ului
template <class T, class E>
class ArgumentMapper
{
public:
    ArgumentMapper(int threadID, vector<string> &testFiles, int exponent, void (*algorithmFunction)(void *arg),
                   string file, pthread_mutex_t &syncMutex, pthread_barrier_t &syncBarrier, sem_t &syncSem,
                   unordered_map<T, unordered_set<E>> &mapExp) : algorithmFunction(algorithmFunction), testFiles(testFiles),
                                                                 syncBarrier(syncBarrier), syncMutex(syncMutex),
                                                                 syncSem(syncSem), mapExp(mapExp)
    {
        this->threadID = threadID;
        this->exponent = exponent;
        this->file = file;
    }

    // Metode de prelucrare
    int getThreadId();
    void setThreadId(int threadID);
    vector<string> *getTestFiles();
    void setTestFiles(vector<string> testFiles);
    int getExponent();
    void setExponent(int exponent);
    string getFile();
    void setFile(string file);
    pthread_mutex_t *getSyncMutex();
    void setSyncMutex(pthread_mutex_t syncMutex);
    pthread_barrier_t *getSyncBarrier();
    void setSyncBarrier(pthread_barrier_t syncBarrier);
    sem_t *getSyncSem();
    void setSyncSem(sem_t syncSem);
    unordered_map<T, unordered_set<E>> *getMapExp();
    void setMapExp(unordered_map<T, unordered_set<E>> mapExp);
    void (*algorithmFunction)(void *arg);
    static double nthRoot(double a, int n);
    static double IntPowFast(double x, int n);
    static constexpr double eps = 1e-14;

    // Proprietatile unui thread Mapper
private:
    int threadID;
    vector<string> &testFiles;
    int exponent;
    string file;
    pthread_barrier_t &syncBarrier;
    pthread_mutex_t &syncMutex;
    sem_t &syncSem;
    unordered_map<T, unordered_set<E>> &mapExp;
};

// Metode simple de prelucrare si obtinere
template <class T, class E>
int ArgumentMapper<T, E>::getThreadId()
{
    return threadID;
}
template <class T, class E>
void ArgumentMapper<T, E>::setThreadId(int threadID)
{
    this->threadID = threadID;
}
template <class T, class E>
vector<string> *ArgumentMapper<T, E>::getTestFiles()
{
    return &testFiles;
}
template <class T, class E>
void ArgumentMapper<T, E>::setTestFiles(vector<string> testFiles)
{
    this->testFiles = testFiles;
}
template <class T, class E>
int ArgumentMapper<T, E>::getExponent()
{
    return exponent;
}
template <class T, class E>
void ArgumentMapper<T, E>::setExponent(int exponent)
{
    this->exponent = exponent;
}
template <class T, class E>
string ArgumentMapper<T, E>::getFile()
{
    return file;
}
template <class T, class E>
void ArgumentMapper<T, E>::setFile(string file)
{
    this->file = file;
}
template <class T, class E>
pthread_mutex_t *ArgumentMapper<T, E>::getSyncMutex()
{
    return &syncMutex;
}
template <class T, class E>
void ArgumentMapper<T, E>::setSyncMutex(pthread_mutex_t syncMutex)
{
    this->syncMutex = syncMutex;
}
template <class T, class E>
pthread_barrier_t *ArgumentMapper<T, E>::getSyncBarrier()
{
    return &syncBarrier;
}
template <class T, class E>
void ArgumentMapper<T, E>::setSyncBarrier(pthread_barrier_t syncBarrier)
{
    this->syncBarrier = syncBarrier;
}
template <class T, class E>
sem_t *ArgumentMapper<T, E>::getSyncSem()
{
    return &syncSem;
}
template <class T, class E>
void ArgumentMapper<T, E>::setSyncSem(sem_t syncSem)
{
    this->syncSem = syncSem;
}
template <class T, class E>
unordered_map<T, unordered_set<E>> *ArgumentMapper<T, E>::getMapExp()
{
    return &mapExp;
}
template <class T, class E>
void ArgumentMapper<T, E>::setMapExp(unordered_map<T, unordered_set<E>> mapExp)
{
    this->mapExp = mapExp;
}

// O clasa utila pentru a modela argumentele Reducer-ului
template <class T, class E>
class ArgumentReducer
{
public:
    ArgumentReducer(int threadID, unordered_set<E> &setReducer, sem_t &syncSem, pthread_barrier_t &syncBarrier,
                    int nrMappers, unordered_map<T, unordered_set<E>> *mapVecExp) : setReducer(setReducer), syncSem(syncSem), syncBarrier(syncBarrier)
    {
        this->threadID = threadID;
        this->nrMappers = nrMappers;
        this->mapVecExp = mapVecExp;
    }
    // Metode de prelucrare
    int getThreadId();
    void setThreadId(int threadID);
    int getNrMappers();
    void setNrMappers(int nrMappers);
    unordered_set<E> *getSetReducer();
    void setSetReducer(unordered_set<E> setReducer);
    sem_t *getSyncSem();
    void setSyncSem(sem_t syncSem);
    pthread_barrier_t *getSyncBarrier();
    void setSyncBarrier(pthread_barrier_t syncSem);
    unordered_map<T, unordered_set<E>> *getMapVecExp();
    void setMapVecExp(unordered_map<T, unordered_set<E>> *mapVecExp);

    // Proprietatile unui thread Reducer
private:
    int threadID;
    int nrMappers;
    unordered_set<E> &setReducer;
    sem_t &syncSem;
    pthread_barrier_t &syncBarrier;
    unordered_map<T, unordered_set<E>> *mapVecExp;
};

// Implementarile unor metode simple de prelucrare si obtinere
template <class T, class E>
int ArgumentReducer<T, E>::getThreadId()
{
    return threadID;
}
template <class T, class E>
void ArgumentReducer<T, E>::setThreadId(int threadID)
{
    this->threadID = threadID;
}
template <class T, class E>
int ArgumentReducer<T, E>::getNrMappers()
{
    return nrMappers;
}
template <class T, class E>
void ArgumentReducer<T, E>::setNrMappers(int nrMappers)
{
    this->nrMappers = nrMappers;
}
template <class T, class E>
unordered_set<E> *ArgumentReducer<T, E>::getSetReducer()
{
    return &setReducer;
}
template <class T, class E>
void ArgumentReducer<T, E>::setSetReducer(unordered_set<E> setReducer)
{
    this->setReducer = setReducer;
}
template <class T, class E>
sem_t *ArgumentReducer<T, E>::getSyncSem()
{
    return &syncSem;
}
template <class T, class E>
void ArgumentReducer<T, E>::setSyncSem(sem_t syncSem)
{
    this->syncSem = syncSem;
}
template <class T, class E>
pthread_barrier_t *ArgumentReducer<T, E>::getSyncBarrier()
{
    return &syncBarrier;
}
template <class T, class E>
void ArgumentReducer<T, E>::setSyncBarrier(pthread_barrier_t syncBarrier)
{
    this->syncBarrier = syncBarrier;
}
template <class T, class E>
unordered_map<T, unordered_set<E>> *ArgumentReducer<T, E>::getMapVecExp()
{
    return mapVecExp;
}
template <class T, class E>
void ArgumentReducer<T, E>::setMapVecExp(unordered_map<T, unordered_set<E>> *mapVecExp)
{
    this->mapVecExp = mapVecExp;
}

// Algoritmul pentru rezolvarea cerintei
// Clasa ArgumentMapper permite asignarea oricarui algoritm implementat intr-o metoda
template <class T, class E>
void constructLists(void *obj)
{
    // Se proceseaza informatiile din obiect
    ArgumentMapper<T, E> *object = (ArgumentMapper<T, E> *)obj;
    ifstream inFile(object->getFile());
    string fileLine;
    getline(inFile, fileLine);
    int index = atoi(fileLine.c_str());
    vector<T> fileValues(index);

    // Se adauga valorile din fisier
    for (int i = 0; i < index; i++)
    {
        getline(inFile, fileLine);
        fileValues.operator[](i) = atoi(fileLine.c_str());
    }

    inFile.close();

    // Fiecare thread Mapper isi populeaza dictionarul
    for (int i = 2; i <= object->getExponent(); i++)
    {
        for (auto iter : fileValues)
        {
            if (iter > 0)
            {
                double finalRes = ArgumentMapper<T, E>::nthRoot(iter, i);
                if (finalRes - floor(finalRes) <= object->eps)
                    object->getMapExp()->operator[](i).insert(iter);
            }
        }
    }
}

// O metoda care implementeaza ridicarea la putere rapida
template <class T, class E>
double ArgumentMapper<T, E>::IntPowFast(double num, int power)
{

    if (power < 0)
    {
        num = 1.0 / num;
        power = -power;
    }

    double res = 1.0;
    int bit = -1;

    for (int temp = power; temp > 0; temp >>= 1)
        bit++;

    for (; bit >= 0; bit--)
    {
        res *= res;
        if (((power >> bit) & 1) != 0)
            res *= num;
    }
    return res;
}

// Metoda care decide daca un numar este putere perfecta
template <class T, class E>
double ArgumentMapper<T, E>::nthRoot(double num, int order)
{
    double x = num;
    while (true)
    {
        double x0 = x;

        // Metoda Newton - Raphson
        x -= (x - num * IntPowFast(x, 1 - order)) / order;

        if (abs(x - x0) <= abs(eps * x0))
            return x;
    }
}

// Metoda specifica Mappers
template <class T, class E>
void *funcMappers(void *arg)
{
    ArgumentMapper<T, E> *mapThread = (ArgumentMapper<T, E> *)arg;
    string fileIn;
    while (true)
    {

        // Fiecare thread citeste pe rand un fisier
        pthread_mutex_lock(mapThread->getSyncMutex());
        if (mapThread->getTestFiles()->empty())
        {
            pthread_mutex_unlock(mapThread->getSyncMutex());
            break;
        }
        else
        {
            fileIn = mapThread->getTestFiles()->back();
            mapThread->getTestFiles()->pop_back();
        }

        // Se proceseaza fisierul obtinut;
        pthread_mutex_unlock(mapThread->getSyncMutex());
        mapThread->setFile(fileIn);
        mapThread->algorithmFunction(mapThread);
    }

    pthread_barrier_wait(mapThread->getSyncBarrier());

    // Thread-urile Mappers "notifica" thread-urile Reducers folosind un semafor partajat
    pthread_mutex_lock(mapThread->getSyncMutex());
    sem_post(mapThread->getSyncSem());
    pthread_mutex_unlock(mapThread->getSyncMutex());
    pthread_exit(NULL);
}

// Metoda specifica Reducers
template <class T, class E>
void *funcReducers(void *arg)
{
    ArgumentReducer<T, E> *mapReducer = (ArgumentReducer<T, E> *)arg;
    pthread_barrier_wait(mapReducer->getSyncBarrier());
    sem_wait(mapReducer->getSyncSem());
    sem_post(mapReducer->getSyncSem());

    // Fiecare Reducer isi populeaza propriul unordered_set
    for (int i = 0; i < mapReducer->getNrMappers(); i++)
    {
        for (auto j : mapReducer->getMapVecExp()[i])
        {
            // Daca puterea (cheia) dictionarului este corecta
            if (j.first == mapReducer->getThreadId() - mapReducer->getNrMappers() + 2)

                // Se pun elementele (duplicatele se elimina)
                for (auto k : j.second)
                    mapReducer->getSetReducer()->insert(k);
        }
    }

    // Fiecare Reducer isi completeaza fisierul de iesire
    ofstream fout("out" + to_string(mapReducer->getThreadId() - mapReducer->getNrMappers() + 2) + ".txt");
    fout << mapReducer->getSetReducer()->size();
    pthread_exit(NULL);
}

void verifyStart(int argc)
{
    if (argc != 4)
    {
        cout << "Numar insuficient de parametri: ./tema1 M R fisier\n";
        exit(1);
    }
}

int main(int argc, char **argv)
{
    verifyStart(argc);
    int err;

    // Elementele de sincronizare
    pthread_mutex_t syncMutex;
    pthread_barrier_t syncBarrierMapper;
    pthread_barrier_t syncBarrierReducer;
    sem_t syncSem;

    int nrMappers = atoi(argv[1]);
    int nrReducers = atoi(argv[2]);
    string testFile = argv[3];
    string line;
    ifstream fin(testFile);
    getline(fin, line);

    pthread_t *threadsMap = new pthread_t[nrMappers];
    pthread_t *threadsReducer = new pthread_t[nrReducers];

    // Elemente de programare defensiva
    if (pthread_mutex_init(&syncMutex, NULL) != 0)
    {
        cout << "Eroare la initializarea mutex-ului";
        exit(1);
    }
    if (pthread_barrier_init(&syncBarrierMapper, NULL, nrMappers) != 0)
    {
        cout << "Eroare la initializarea barierei Mapper";
        exit(1);
    }
    if (pthread_barrier_init(&syncBarrierReducer, NULL, nrReducers) != 0)
    {
        cout << "Eroare la initializarea barierei Reducer";
        exit(1);
    }
    if (sem_init(&syncSem, 0, 0) != 0)
    {
        cout << "Eroare la initializarea semaforului";
        exit(1);
    }

    // Structurile de date necesare rezolvarii cerintei
    vector<ArgumentMapper<int, int> *> argsMapper(nrMappers);
    vector<ArgumentReducer<int, int> *> argsReducer(nrReducers);
    unordered_map<int, unordered_set<int>> *mapExp =
        new unordered_map<int, unordered_set<int>>[nrMappers];
    unordered_set<int> *sets = new unordered_set<int>[nrReducers];
    vector<string> testFiles;

    while (getline(fin, line))
    {
        testFiles.push_back(line);
    }

    fin.close();

    // Ciclul care porneste toate thread-urile
    for (int i = 0; i < nrMappers + nrReducers; i++)
    {
        if (i <= nrMappers - 1)
        {
            // Se paseaza algoritmul constructLists in constructor
            argsMapper[i] = new ArgumentMapper<int, int>(i, testFiles, nrReducers + 1, &constructLists<int, int>, "", syncMutex, syncBarrierMapper, syncSem, mapExp[i]);
            err = pthread_create(&threadsMap[i], NULL, funcMappers<int, int>, argsMapper[i]);
        }
        if (err)
        {
            printf("Eroare la crearea thread-ului %d\n", i);
            exit(-1);
        }
        else if (i >= nrMappers)
        {
            argsReducer[i - nrMappers] = new ArgumentReducer<int, int>(i, sets[i - nrMappers], syncSem, syncBarrierReducer, nrMappers, mapExp);
            err = pthread_create(&threadsReducer[i - nrMappers], NULL, funcReducers<int, int>, argsReducer[i - nrMappers]);
        }
    }

    // Ciclul care asteapta toate thread-urile sa se intoarca din executie
    for (int i = 0; i < nrMappers + nrReducers; i++)
    {
        if (i <= nrMappers - 1)
        {
            err = pthread_join(threadsMap[i], NULL);
        }
        if (err)
        {
            printf("Eroare la asteptarea thread-ului %d\n", i);
            exit(-1);
        }
        else if (i >= nrMappers)
        {
            err = pthread_join(threadsReducer[i - nrMappers], NULL);
        }
    }

    // Eliberarea memoriei in mod corespunzator
    for (int i = 0; i < nrReducers; i++)
        delete argsReducer[i];
    for (int i = 0; i < nrMappers; i++)
        delete argsMapper[i];
    pthread_mutex_destroy(&syncMutex);
    pthread_barrier_destroy(&syncBarrierReducer);
    pthread_barrier_destroy(&syncBarrierMapper);
    sem_destroy(&syncSem);
    delete[] mapExp;
    delete[] sets;
    delete[] threadsMap;
    delete[] threadsReducer;
}