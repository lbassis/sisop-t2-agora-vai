#include <superblock.h>
#include <t2fs.h>
#include <disk_handler.h>

RECORDS_LIST *open_files;
RECORDS_LIST *open_dirs;
int has_initialized = 0;

void init() {
  current_path = malloc(sizeof(int));
  current_path[0] = '/';

  open_files = newList();
  open_dirs = newList();
}

int identify2 (char *name, int size) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  // apenas por motivo de teste, aqui tem chamadas às funções do superblock.c
  // printf("\n== Printando diretamente do identify2 ==s\n");
  // diskId();
  // SUPERBLOCK s;
  // readSuperBlock(&s);
  // printSuperBlock(s);
}

FILE2 create2 (char *filename) {

  // considerando que vai criar sempre no current_path (nao pode incluir path no nome do arquivo):

  // 1. encontra o primeiro cluster livre e atribui a ele ff ff ff ff
  // 2. abre o diretorio do current path
  // 2.1 se nao encontrar -> erro
  // 3. adiciona a nova entrada ao diretorio: typeval = 1, name = filename, bytesfilesize = 0, firstcluster = valor encontrado em 1.
  // 4. fecha o diretorio

  // acredito que sempre crie vazio, entao nao precisa mexer no cluster dele
}

int delete2 (char *filename) {

  // considerando que vai excluir sempre do current_path (nao pode incluir o path no nome do arquivo):

  // 1. abre o diretorio do current path
  // 2. le o registro com name == filename
  // 2.1 se nao encontrar -> erro
  // 3. encontra a entrada do firstCluster na fat
  // 4. enquanto (valor_lido_na_fat != ff ff ff ff:
  //                proximo_cluster = valor_lido_na_fat
  //                zera (proximo_cluster) (escreve 0 em tudo [talvez isso nao seja necessario])
  //                valor_lido_na_fat <- 0
  //                valor_lido_na_fat <- proximo_cluster
  //  (isso vai apagar todos os clusters e encadeamentos MENOS O PRIMEIRO!!!)
  // 5. zera (firstCluster)
  // 6. remove do diretorio o registro com name == filename
  // 7. fecha o diretorio

}

FILE2 open2 (char *filename) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_files);

  if (length >= MAX_ITEMS_IN_OPEN_LIST) {
    printf("\n=======\nLista com nro máximo de elementos, man\n=======\n");
    return -1;
  }

  // aqui tem que ver se o arquivo com este filename existe
  // se não existir, erro!
  // se existir, pega a entrada dele

  // criando um arquivo dummy
  struct t2fs_record dummy_rec;

  dummy_rec.TypeVal = 1;
  strcpy(dummy_rec.name, filename);
  dummy_rec.bytesFileSize = 100;
  dummy_rec.firstCluster = 4;

  int handler_available = get_fisrt_handler_available(open_files, MAX_ITEMS_IN_OPEN_LIST);
  printf("\n=====\n\nhandler to add: %i\n\n", handler_available);

  GENERIC_FILE dummy_generic_file;

  dummy_generic_file.record = dummy_rec;
  dummy_generic_file.handler = handler_available;
  dummy_generic_file.pointer = 0;

  // insere o arquivo
  insert_record(&open_files, dummy_generic_file);

  length = list_length(open_files);

  // printf("\nAdded file: %s", dummy.name);
  print_records(open_files);
  printf("\nList length: %i\n", length);

  return 0;
}

int close2 (FILE2 handle) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_files);

  if (handle < 0 || handle >= MAX_ITEMS_IN_OPEN_LIST) {
    printf("handle fora dos limites ou sei lá, man\n");
    return -1;
  }

  printf("\n=====\n\nhandler to remove: %i\n\n", handle);

  // print_records(open_files);
  // length = list_length(open_files);
  // printf("\nList length: %i\n\n", length);

  if (remove_record_at_index(&open_files, handle) != 0) {
    printf("Erro ao fechar arquivo %i\n", handle);
    return -1;
  }

  length = list_length(open_files);

  // printf("\nAdded file: %s", dummy.name);
  print_records(open_files);
  printf("\nList length: %i\n", length);

  return 0;
}

int read2 (FILE2 handle, char *buffer, int size) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (handle < 0 || handle > MAX_ITEMS_IN_OPEN_LIST) {
    printf("handle fora dos limites, man\n");
    return -1;
  }
  struct t2fs_record *rec;
  rec = (struct t2fs_record *) get_record_at_index(open_files, 0);

  if (rec == NULL) {
    printf("O handle %i non ecziste\n", handle);
    return -1;
  }

  // achou o arquivos
  printf("Reading %s\n", rec->name);

  // aqui falta colocar o conteúdo do arquivo no buffer, creio eu
  // (ler "size" bytes a partir do current_pointer)

  return 0;
}

int write2 (FILE2 handle, char *buffer, int size) {}

int truncate2 (FILE2 handle) {

  // 1. encontra o arquivo com handle == handle na lista de arquivos abertos
  // 1.2 se nao encontrar -> erro
  // 2. encontra o cluster onde se encontra o ponteiro do record
  // 3. initialCluster <- entrada na fat do cluster encontrado em 2
  // 4. enquanto initialCluster != ff ff ff ff
  //                zera o conteudo do initialCluster
  //                proxCluster <- entrada do initialCluster na fat
  //                muda a entrada do initialCluster na fat pra ff ff ff ff
  //                initialCluster <- proxCluster
  // 5. atualiza tamanho do arquivo

}

int seek2 (FILE2 handle, unsigned int offset) {

  // 1. encontra o arquivo com handle == handle na lista de arquivos abertos
  // 1.2 se nao encontrar -> erro
  // 2. posiciona o ponteiro do record na posiçao anterior + offset (????)

}

int mkdir2 (char *pathname) {


  // considerando que vai criar sempre no current_path (nao pode incluir path no nome do diretorio):

  // 1. encontra o primeiro cluster livre e atribui a ele ff ff ff ff
  // 2. abre o diretorio do current path
  // 2.1 se nao encontrar -> erro
  // 3. adiciona a nova entrada ao diretorio: typeval = 2, name = filename, bytesfilesize = 0, firstcluster = valor encontrado em 1.
  // 4. fecha o diretorio

  // acredito que sempre crie vazio, entao nao precisa mexer no cluster dele
}

int rmdir2 (char *pathname) {

  // considerando que vai excluir sempre do current_path (nao pode incluir o path no nome do diretorio):

  // 1. abre o diretorio do current path
  // 2. le o registro com name == filename
  // 2.1 se nao encontrar -> erro
  // 3. encontra a entrada do firstCluster na fat
  // 4. zera (firstCluster)
  // 5. remove do diretorio o registro com name == filename
  // 7. fecha o diretorio

}

int chdir2 (char *pathname) {
    current_path = strcat(current_path, pathname);
}

int getcwd2 (char *pathname, int size) {}

DIR2 opendir2 (char *pathname) {}

int readdir2 (DIR2 handle, DIRENT2 *dentry) {}

int closedir2 (DIR2 handle) {}
