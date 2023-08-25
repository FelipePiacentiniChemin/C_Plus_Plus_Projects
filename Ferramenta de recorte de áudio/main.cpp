/*
TITULO: Pegando Onda
Turma: 4311
Alunos: Felipe Piacentini Chemin e Cainã Schaurich Borges
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <locale>

using namespace std;
struct WaveStruct{             // struct com todos os dados do header
    char ChunkID[4];
    uint32_t ChunckSize;
    char Format[4];
    char Subchunk1ID[4];
    uint32_t Subchunck1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    char Subchunck2ID[4];
    uint32_t Subchunck2Size;
};

string showID(char dado[], int bytes)   // função para retornar uma string atraves de um char
{
    string resposta;
    for(int x=0; x<bytes; x++){
        resposta.push_back(dado[x]);
    }
    return resposta;
}

int main()
{
    string nome;    // string com o nome do arquivo
    char leitor;    // 1 byte

    setlocale(LC_ALL,"");

    ifstream inFile;    // inFile é o arquivo de leitura dos dados

    cout << "Qual é o nome do arquivo que você deseja abrir?" << endl;
    cin >> nome; cout << endl;
    inFile.open(nome+".wav", ios::in|ios::binary);  // abre o arquivo .wav para leitura em binario
    if (! inFile)
       { cerr << "Arquivo \""<< nome << "\" não existe ou não é do formato wav" << endl;    // avisa se não for possivel abrir o arquivo
        return -1;
       }

    WaveStruct header;

    inFile.read((char *)&header,sizeof(header));    // le 44 bytes do arquivo e coloca no struct header

    cout << "ChunkID: " << showID(header.ChunkID, 4) << endl;
    cout << "Tamanho do chunk: " << header.ChunckSize << " bytes" << endl;
    cout << "Formato: " << showID(header.Format, 4) << endl;
    cout << "Subchunk1ID: " << showID(header.Subchunk1ID, 4) << endl;
    cout << "Tamanho do Subchunk1: " << header.Subchunck1Size << " (16 = PCM)" << endl;
    cout << "Formato de audio: " << header.AudioFormat << endl;
    cout << "Número de canais: " << header.NumChannels << " (1 = mono e 2 = stereo)" << endl;
    cout << "Taxa de amostragem: " << header.SampleRate << endl;
    cout << "Taxa de bytes: " << header.ByteRate << endl;
    cout << "Block align: " << header.BlockAlign << endl;
    cout << "Bits por amostragem: " << header.BitsPerSample << endl;
    cout << "Subchunk2ID: " << showID(header.Subchunck2ID, 4) << endl;
    cout << "Tamanho do Subchunk2: " << header.Subchunck2Size << " bytes" << endl;

    float beginning, ending;

    cout << endl << "Tempo inicial desejado em segundos: ";
    cin >> beginning;

    cout << endl << "Tempo final desejado em segundos: ";
    cin >> ending;
    ofstream outFile;

    outFile.open(nome+"_cut.wav", ios::out|ios::binary);    // abre o arquivo _cut.wav para escrever em binario
    if (! outFile){
       cout << "Arquivo " << nome << " não pode ser crido" << endl;     // avisa se não for possivel abrir o arquivo
       return -1;
    }

    outFile.write((char *)&header,sizeof(header));  // copia o header do arquivo original para o _cut

    inFile.seekg(header.ByteRate*beginning);    // vai para a posição do segundo da musica desejado

    int audioStart = outFile.tellp();   // guarda quantidade de bytes antes dos dados

    for(int x=0; x<(header.ByteRate*(ending-beginning)); x++){  // le e escreve byte por byte os dados
        inFile.read((char *)&leitor, 1);
        outFile.write((char*)&leitor, 1);
    }

    int audioEnd = outFile.tellp();    // guarda quantidade de bytes depois dos dados

    outFile.seekp(4, ios::beg);     // vai para a posição 4 bytes depois do começo do arquivo (chunkSize)
    header.ChunckSize = (audioEnd-8);
    outFile.write((char *)&header.ChunckSize,4);    // reescreve o tamanho do chunk
    cout << "\nNovo tamanho do chunk: " << header.ChunckSize << endl;   // mostra o novo tamanho

    outFile.seekp(audioStart-4);    // vai para a posição 4 bytes antes do começo dos dados (subChunk2Size)
    header.Subchunck2Size = (audioEnd-audioStart);
    outFile.write((char *)&header.Subchunck2Size,4);    // reescreve o tamanho do subChunk2
    cout << "Novo tamanho do Subchunk2: " << header.Subchunck2Size << endl;  // mostra o novo tamanho

    char meta [298];    // variavel para guardar os metadados

    inFile.seekg(-256, ios::end);   // le o nome do album dentro dos metadados
    inFile.read(meta, 32);
    cout << "\nAlbum: " << showID(meta, 32) << endl;

    inFile.seekg(-278 , ios::end);  // le o nome da musica dentro dos metadados
    inFile.read(meta, 13);
    cout << "Musica: " << showID(meta, 13) << endl;

    inFile.seekg(-12 , ios::end);   // le o nome do artista dentro dos metadados
    inFile.read(meta, 11);
    cout << "Artista: " << showID(meta, 11) << endl;

    outFile.seekp(0, ios::end);

    inFile.seekg(-298, ios::end);   // passa todos os metadados para o arquivo de saida
    inFile.read(meta, 298);
    outFile.write((char*)&meta, 298);

    outFile.close();
    inFile.close();
    return 0;
}
