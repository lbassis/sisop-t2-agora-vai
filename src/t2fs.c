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

  // ASSIS DOIDAO DIZ:
  //
  // considerando que vai criar sempre no current_path (nao pode incluir path no nome do arquivo):

  // 1. encontra o primeiro cluster livre e atribui a ele ff ff ff ff
  // 2. abre o diretorio do current path
  // 2.1 se nao encontrar -> erro
  // 3. adiciona a nova entrada ao diretorio: typeval = 1, name = filename, bytesfilesize = 0, firstcluster = valor encontrado em 1.
  // 4. fecha o diretorio

  // acredito que sempre crie vazio, entao nao precisa mexer no cluster dele
  //
  // ================================================================================
  //
  // BORANGA DOIDAO DIZ:
  //
  // o filename de entrada é ABSOLUTO, conforme t2fs.h linha 91

  // 1. encontra a primeira entrada da fat livre e atribui a ela ff
  // 2. vê se o diretório do dir pai existe (pega as entradas dele)
  // 2.1 se nao encontrar -> erro
  // 2.2 cria um GENERIC_FILE e um record pra este arquivo novo
  // 3. adiciona a nova entrada às entradas do diretório diretorio: typeval = 1, name = filename, bytesfilesize = 0, firstcluster = valor encontrado em 1.
  //    escreve no disco
  //    escreve setor? ou cluster inteiro (mais fácil, escreve a lista toda) ?
  // 4. fecha o diretorio

  // de fato, cria vazio, entao nao precisa mexer no cluster dele
  
  // pega o nome do pai
  char *name = malloc(sizeof(filename));
  char *father_path = malloc(sizeof(filename));
  
  father_path = get_father_dir_path(filename);
  name = (char *) get_filename_from_path(filename);
  
  int cluster_index = get_initial_cluster_from_path(father_path);

  // pega lista de entradas do diretório pai do arquivo a ser criado
  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(cluster_index, &files_in_father_dir);
  
  // checa se já existe arquivo com este nome
  if (find_record(files_in_father_dir, name) != NULL) {
    printf("\nErro ao criar %s.\nJá existe um arquivo com este nome neste diretório.\n\n", filename);
    return -1;
  }
  
  // ========== criação do novo elemento da lista ==========
  
  // firstCluster
  int fat_entry = get_first_fat_entry_available();
  
  if (set_fat_entry(fat_entry, -1) == -1 || fat_entry < 0) {
    printf("Erro ao setar fat entry como ff\n");
    return -1;
  }
  
  // cria um record
  struct t2fs_record *record = malloc(sizeof(struct t2fs_record));
  record->TypeVal = 1; // arquivo regular
  strcpy(record->name, name);
  
  record->bytesFileSize = 0;
  record->firstCluster = fat_entry; // index da fat entry alocada pra esse record
  
  // cria um elemento pra botar na lista
  GENERIC_FILE new_file;
  new_file.record = *record;
  new_file.handler = -1; // bota -1 pq nesse caso nao faz sentido usar o handler
  new_file.pointer = 0;
  
  // insere o arquivo
  insert_record(&files_in_father_dir, new_file);
  
  // falta escrever as entradas do diretorio pai de volta pro seu cluster
  write_list_of_records_to_cluster(files_in_father_dir, cluster_index);
  
  // daqui pra baixo é só pra teste mesmo!
  RECORDS_LIST *dir = newList();
  read_all_records(cluster_index, &dir);
  
  print_records(dir);
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
  
  // confere se ainda tem espaço na lista
  if (length >= MAX_ITEMS_IN_OPEN_LIST) {
    printf("\n=======\nLista com nro máximo de elementos, man\n=======\n");
    return -1;
  }
  
  // se chegou aqui, pode colocar na lista
  
  char *filename_without_path = malloc(sizeof(filename));
  char *father_path = malloc(sizeof(filename));
  
  father_path = get_father_dir_path(filename);
  filename_without_path = (char *) get_filename_from_path(filename);
  
  int cluster_index = get_initial_cluster_from_path(father_path);

  // pega lista de entradas do diretório pai do arquivo em questão
  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(cluster_index, &files_in_father_dir);
  
  struct t2fs_record *record;
  record = find_record(files_in_father_dir, filename_without_path);
  
  if (record == NULL) {
    printf("Erro ao pegar record do arquivo %s\n", filename_without_path);
    return -1;
  }
  
  int handler_available = get_fisrt_handler_available(open_files, MAX_ITEMS_IN_OPEN_LIST);
  
  // provavelmente não vai cair aqui pois se não tem espaço na lista já deve ter caído fora
  // mas é bom garantir..
  if (handler_available < 0) {
    printf("Erro: todos handles estão ocupados\n");
    return -1;
  }
  
  // cria um elemento pra botar na lista
  GENERIC_FILE generic_file;
  generic_file.record = *record;
  generic_file.handler = handler_available;
  generic_file.pointer = 0;
  
  // insere o arquivo
  insert_record(&open_files, generic_file);

  length = list_length(open_files);

  printf("\n===== open files =====\n");
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

  if (remove_record_at_index(&open_files, handle) != 0) {
    printf("Erro ao fechar arquivo %i\n", handle);
    return -1;
  }
  
  return 0;
}

int read2 (FILE2 handle, char *buffer, int size) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }
//
//   if (handle < 0 || handle > MAX_ITEMS_IN_OPEN_LIST) {
//     printf("handle fora dos limites, man\n");
//     return -1;
//   }
//   struct t2fs_record *rec;
//   rec = (struct t2fs_record *) get_record_at_index(open_files, 0);
//
//   if (rec == NULL) {
//     printf("O handle %i non ecziste\n", handle);
//     return -1;
//   }
//
//   // achou o arquivos
//   printf("Reading %s\n", rec->name);

  // aqui falta colocar o conteúdo do arquivo no buffer, creio eu
  // (ler "size" bytes a partir do current_pointer)

  return 0;
}

int write2 (FILE2 handle, char *buffer, int size) {

  // início padrão procurando pelo handler na lista de abertos

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_files);

  if (handle < 0 || handle >= MAX_ITEMS_IN_OPEN_LIST) {
    printf("handle out of range\n");
    return -1;
  }

  printf("\n=====\n\nhandler to write: %i\n\n", handle);

  struct t2fs_record *rec;
  rec = (struct t2fs_record *) get_record_at_index(open_files, 0);

  if (rec == NULL) {
    printf("O handle %i n existe\n", handle);
    return -1;
  }

  // fim do "início padrão"

  // 1. enquanto initialCluster != ff ff ff ff
  // 2.    2.1 proxCluster <- entrada do initialCluster na fat
  //       2.2 quando proxCluster for == ff ff ff ff
  // 3.   se houver algum setor livre no cluster:
  //      3.1 escreve 256 bytes do buffer no setor livre
  //      3.2 desconta 256 bytes do local_buffer
  //      3.3 checa o próximo setor do clusters
  //      3.4 se houver setor livre, volta para 3.1 senão
  // 4. aloca um cluster novo, encadeia e cluster novo = EOF
  // 5. vá_para 3
  // 6. se buffer não possui mais bytes a serem escritos, retorna 0



}

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
  // pra testes apenas
  // printf("\n=======\n\npath: \t%s\n", pathname);
  // printf("father:\t%s\n\n", get_father_dir_path(pathname));

  char *filename = malloc(sizeof(pathname));
  char *father_path = malloc(sizeof(pathname));

  father_path = get_father_dir_path(pathname);
  filename = (char *) get_filename_from_path(pathname);

  printf("father: %s\n", father_path);

  printf("will get cluster index\n");
  int cluster_index = get_initial_cluster_from_path(father_path);

  printf("cluster index: %i\n", cluster_index);

  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(cluster_index, &files_in_father_dir);

  struct t2fs_record *record;
  record = find_record(files_in_father_dir, filename);

  if (record == NULL) {
    printf("Erro ao pegar record do arquivo %s\n", filename);
    return -1;
  }

  print_records(files_in_father_dir);
  print_record(*record);

  // considerando que vai criar sempre no current_path (nao pode incluir path no nome do diretorio):

  // 1. encontra o primeiro cluster livre e atribui a ele ff ff ff ff
  // 2. abre o diretor  io do current path
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

DIR2 opendir2 (char *pathname) {

  /*
  1. abre o diretório do current path
  2. lê o registro com name == filename
      2.1 se não encontrar := erro
  3. encontra a entrada da firstCluster na fat
  4. posiciona o ponteiro de entradas(current) no firstCluster
  5. retorna handle(identificador)
  */
}

int readdir2 (DIR2 handle, DIRENT2 *dentry) {}

int closedir2 (DIR2 handle) {
  /*
   1. encontra o arquivo com handle == handle na lista de arquivos abertos
   1.2 se nao encontrar -> erro
   2. encontra o cluster onde se encontra o ponteiro do record
   3. initialCluster <- entrada na fat do cluster encontrado em 2
   4. tira handle da lista de arquivos abertos
   5. free ponteiro record ??? ???
  */
}
