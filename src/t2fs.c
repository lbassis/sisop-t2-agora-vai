#include <superblock.h>
#include <t2fs.h>
#include <disk_handler.h>
#include <math.h>
#include <limits.h>
#include <data.h>

// nao sei se isso vai aqui mas azar
int min(a,b) {
  int result = (a < b) ? a : b;
  return result;
}

struct t2fs_superbloco *superblock;


RECORDS_LIST *open_files;
RECORDS_LIST *open_dirs;
int has_initialized = 0;

void init() {
  superblock = malloc(sizeof(struct t2fs_superbloco));
  readSuperBlock(superblock);

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
  
  int i = 0;

  // while (i < size) {
  //
  //   if (name+i != NULL)
  //     printf("%c", name[i]);
  //
  //   else
  //     return -1;
  //
  //     i++;
  //   }
  
  printf("\nAugusto Boranga\nLucas Assis\nOctavio Arruda\n\n");

return 0;
}

FILE2 create2 (char *filename) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (strcmp(filename, "/") == 0) {
    //printf("Erro: tentou criar o root\n");
    return -1;
  }

  char *name = malloc(sizeof(filename));
  char *father_path = malloc(sizeof(filename));

  father_path = get_father_dir_path(filename);
  name = (char *) get_filename_from_path(filename);

  int cluster_index = get_initial_cluster_from_path(father_path);

  // pega lista de entradas do diretório pai do arquivo a ser criado
  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(cluster_index, &files_in_father_dir);

  // checa se já existe arquivo com este nome
  struct t2fs_record *rec = find_record(files_in_father_dir, name);
  if (rec != NULL && rec->TypeVal == TYPEVAL_REGULAR) { //   if (rec != NULL && rec->TypeVal == 1  // era um
    //printf("\nErro ao criar arquivo %s.\nJá existe um arquivo com este nome neste diretório.\n\n", filename);
    return -1;
  }

  // ========== criação do novo elemento da lista ==========

  // firstCluster
  int fat_entry = get_first_fat_entry_available();

  if (set_fat_entry(fat_entry, -1) == FAT_ENTRY_OCCUPIED || fat_entry < 0) { //   if (set_fat_entry(fat_entry, -1) == -1 || fat_entry < 0) //
    //printf("Erro ao setar fat entry como ff\n");
    return -1;
  }

  // cria um record
  struct t2fs_record *record;
  record = (struct t2fs_record *) create_record(TYPEVAL_REGULAR, (char *) name, 0, fat_entry); // create_record(1, (char *) name, 0, fat_entry);

  // cria um elemento pra botar na lista
  GENERIC_FILE *new_file;
  new_file = (GENERIC_FILE *) create_generic_new_file(record, -1, 0);

  // insere o arquivo
  insert_record(&files_in_father_dir, *new_file);

  // falta escrever as entradas do diretorio pai de volta pro seu cluster
  write_list_of_records_to_cluster(files_in_father_dir, cluster_index);

  // daqui pra baixo é só pra teste mesmo!
  RECORDS_LIST *dir = newList();
  read_all_records(cluster_index, &dir);

  // print_records(dir);
}

int delete2 (char *filename) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  //printf("caminho absoluto: %s\n", current_path);
  char *absolute_file_path = malloc(sizeof(char)*(1 + strlen(current_path) + strlen(filename)));
  strcpy(absolute_file_path, current_path);
  strcat(absolute_file_path, filename);
  FILE2 file = open2(absolute_file_path);
  seek2(file, 0);
  truncate2(file);

  GENERIC_FILE *deleted =  get_record_at_index(open_files, file);


  if (deleted != NULL) {
    set_fat_entry(deleted->record.firstCluster, 0);


    if (strcmp(filename, "/") == 0 || strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
      return -1;
    }

    char *name = malloc(sizeof(filename));

    name = (char *) get_filename_from_path(filename);

    int cluster_index = get_initial_cluster_from_path(current_path);

    RECORDS_LIST *files_in_dir = newList();
    read_all_records(cluster_index, &files_in_dir);

    //printf("printando os records do pai que eu achei:\n");
    // print_records(files_in_dir);

    GENERIC_FILE *found_record;
    found_record = (GENERIC_FILE*)get_record_at_filename(files_in_dir, filename);
    // print_record2(found_record->record);


    if (remove_record_at_filename(&files_in_dir, found_record->record.name) == 0) {
      write_list_of_records_to_cluster(files_in_dir, cluster_index);
      close2(file);
      return 0;
    }
    else {
      close2(file);
      return -1;
    }
    //print_records(files_in_father_dir);
  }

  else {
    close2(file);
    return -1;
  }
}

FILE2 open2 (char *filename) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_files);

  // confere se ainda tem espaço na lista
  if (length >= MAX_ITEMS_IN_OPEN_LIST) {
    // printf("\n=======\nLista com nro máximo de elementos, man\n=======\n");
    return -1;
  }

  // se chegou aqui, pode colocar na lista
  char *absolute_file_path = malloc(sizeof(char)*(1 + strlen(current_path) + strlen(filename)));
  strcpy(absolute_file_path, current_path);
  strcat(absolute_file_path, filename);

  char *father_path = malloc(sizeof(absolute_file_path));
  father_path = get_father_dir_path(absolute_file_path);

  int cluster_index = get_initial_cluster_from_path(father_path);

  // pega lista de entradas do diretório pai do arquivo em questão
  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(cluster_index, &files_in_father_dir);

  struct t2fs_record *record;
  record = find_record(files_in_father_dir, filename);

  if (record == NULL) {
    // printf("Erro ao pegar record do arquivo %s\n", filename);
    return -1;
  }

  int handler_available = get_fisrt_handler_available(open_files, MAX_ITEMS_IN_OPEN_LIST);

  // provavelmente não vai cair aqui pois se não tem espaço na lista já deve ter caído fora
  // mas é bom garantir..
  if (handler_available < 0) {
    // printf("Erro: todos handles estão ocupados\n");
    return -1;
  }

  // cria um elemento pra botar na lista
  GENERIC_FILE *generic_file;
  generic_file = (GENERIC_FILE *) create_generic_new_file(record, handler_available, 0);
  generic_file->path = absolute_file_path;

  // insere o arquivo
  insert_record(&open_files, *generic_file);

  length = list_length(open_files);

  // printf("\n===== open files =====\n");
  // print_records(open_files);
  // printf("\nList length: %i\n", length);
  // printf("\n======================\n");

  return handler_available;
}

int close2 (FILE2 handle) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_files);

  if (handle < 0 || handle >= MAX_ITEMS_IN_OPEN_LIST) {
    //printf("handle fora dos limites ou sei lá, man\n");
    return -1;
  }

  if (remove_record_at_index(&open_files, handle) != 0) {
    //printf("Erro ao fechar arquivo %i\n", handle);
    return -1;
  }

  return 0;
}

int read2 (FILE2 handle, char *buffer, int size) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (handle < 0 || handle > MAX_ITEMS_IN_OPEN_LIST) {
    //printf("handle fora dos limites, man\n");
    return -1;
  }

  GENERIC_FILE *file;
  file = get_record_at_index(open_files, handle);

  if (file == NULL) {
    //printf("O handle %i non ecziste\n", handle);
    return -1;
  }

  else {
    //// essas coisas tem que vir do Superbloco
    int sectors_per_cluster = superblock->SectorsPerCluster; // era 4
    int sector_size = SECTOR_SIZE; // era 256, essa constante tá uma porra

    //////////////////////////////////////////

    int occupied_clusters;
    int bytes_by_cluster = sectors_per_cluster*sector_size; // isso vai ser uma variavel global calculada na leitura do superbloco!!!!!!!!!!!!!!
    occupied_clusters = ceil2((float)file->record.bytesFileSize/bytes_by_cluster);

    ////// printf("esse arquivo ocupa %d clusters com %d\n", occupied_clusters, file->record.bytesFileSize);

    int already_read = 0; // bytes já lidos

    int cluster_offset = floor2((float)file->pointer/bytes_by_cluster);
    int bytes_offset = file->pointer % bytes_by_cluster;

    int current_cluster = file->record.firstCluster;
    int next_cluster = read_fat_entry(file->record.firstCluster);
    int remaining_clusters = occupied_clusters - cluster_offset;

    char cluster_read[(superblock->SectorsPerCluster) * SECTOR_SIZE]; // tamanho dum cluster em bytes (é pra pegar do superbloco!!!) // O VALOR ERA cluster_read[1000]
    unsigned int max_bytes_to_read = min(file->record.bytesFileSize - file->pointer, size);
    unsigned int bytes_to_read;
    int clusters_already_read = cluster_offset+1;
    // (bytes_by_cluster-bytes_offset)*remaining_clusters

    ////// printf("max bytes to read: %d\n", max_bytes_to_read);

    while (cluster_offset > 0) { // se tem que pular algum
      current_cluster = read_fat_entry(current_cluster);
      bytes_offset = file->pointer % (cluster_offset*bytes_by_cluster);
      cluster_offset--;
      next_cluster = read_fat_entry(current_cluster);
      remaining_clusters--;
      clusters_already_read++;
    }

    while (already_read < max_bytes_to_read) { // aqui que a magia acontece (depois tem que conferir se nao passou do tamanho do arquivo!!!!!!!!!!!!!!!!!!!!!)

      bytes_to_read = min(clusters_already_read*bytes_by_cluster - file->pointer, size-already_read); // 1024? sectorsPerCluster * SECTOR_SIZE? não sei se fica bom mudar isso não

      read_cluster(current_cluster, cluster_read); // aqui o cluster_read tem o cluster atual
      memcpy(buffer+already_read, cluster_read+bytes_offset, bytes_to_read); // copia tudo que ainda tem pra copiar do buffer

      already_read += bytes_to_read; // leu o que faltava do cluster
      file->pointer += bytes_to_read; // atualiza o ponteiro do arquivo
      bytes_offset = file->pointer % bytes_by_cluster;
      clusters_already_read++;

      if (next_cluster != -1) { // hahahahhahahahauhdasuidhasidhashdiadsaihdsaui isso é o ffffffff
        current_cluster = next_cluster; // vai pro proximo cluster
        next_cluster = read_fat_entry(current_cluster); // prepara o proximo tb
      }

      // //// printf("already_read: %d\nbytes_to_read:%d\n", already_read, bytes_to_read);

    }

    ////// printf("leu: \n%s\n", buffer);

    return SUCESS; // era 0
  }
}

int write2 (FILE2 handle, char *buffer, int size) {

  GENERIC_FILE *file;
  file = get_record_at_index(open_files, handle);

  if(file == NULL){
    // printf("handle %i n existe\n", handle);
    return ERROR; // era -1
  }

  int relative_pointer, current_cluster, bytes_available, buffer_pointer = 0, cluster_is_full = 0;

  current_cluster = find_cluster_from_pointer(file);

  // enquanto ainda houver algo a ser escrito
  while (size > 0) {
    // printf("\n--- FALTA ESCREVER %i bytes ---\n", size);

    relative_pointer = calculate_relative_pointer(file);

    int bytes_per_cluster = SECTOR_SIZE*superblock->SectorsPerCluster;
    bytes_available = bytes_per_cluster - relative_pointer;


    // se houver ao menos 1 byte vazio no cluster
    if (bytes_available > 0 && !cluster_is_full) {
      // printf("-> cluster onde o pointer está é: %i\n", current_cluster);
      // printf("-> relative pointer: %i\n", relative_pointer);
      // printf("-> %i bytes available\n", bytes_available);
      int ammount_to_write = size;
      // seta quanto deve escrever:
      // se houver mais bytes pra serem escritos do que disponíveis, escreve o que couber (bytes_available)
      // se houver mais bytes disponíveis do que pra escrever, escreve só o que precisa (size)
      if (bytes_available < size) {
        // printf("-> tem mais pra escrever do que cabe neste cluster.\n");
        ammount_to_write = bytes_available;
        cluster_is_full = 1;
      }

      // checa se deu erro ao escrever
      if (write_cluster_partially(current_cluster, buffer, buffer_pointer, relative_pointer, ammount_to_write) != 0) {
        // printf("Erro ao escrever\n");
        return ERROR; // era -1
      }
      // printf("-> will write from buffer starting at index %i\n", buffer_pointer);
      // printf("-> wrote %i bytes\n", ammount_to_write);

      size -= ammount_to_write;
      file->record.bytesFileSize += ammount_to_write;
      file->pointer += ammount_to_write;
      buffer_pointer += ammount_to_write;

      // atualiza a entrada no diretorio pai pra refletir o nro de bytes escritos
      if (update_bytesFileSize_in_father_dir(file) != 0) {
        // printf("Erro ao atualizar bytesFileSize do arquivo: %s\n", file->record.name);
        return -1;
      }

    } else {
      // se não houver mais espaço na cluster,
      // checa se já existe um próximo cluster alocado

      int next_cluster = read_fat_entry(current_cluster);

      // se o cluster atual é o último alocado pro arquivo
      if (next_cluster == -1) {
        // aloca outro cluster
        next_cluster = get_first_fat_entry_available();

        set_fat_entry(current_cluster, next_cluster);
        set_fat_entry(next_cluster, FAT_ENTRY_OCCUPIED);

        // printf("-> não há mais bytes available no cluster %i\n", current_cluster);
        // printf("-> alocarei o cluster %i\n", next_cluster);
      }

      // avança para o próximo cluster
      current_cluster = next_cluster;
      cluster_is_full = 0;
    }
  }

  return SUCESS;
}

int truncate2 (FILE2 handle) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (handle < 0 || handle > MAX_ITEMS_IN_OPEN_LIST) {
    //printf("handle fora dos limites, man\n");
    return ERROR;
  }

  GENERIC_FILE *file;
  file = get_record_at_index(open_files, handle);

  if (file == NULL) {
    //printf("O handle %i non ecziste\n", handle);
    return ERROR;
  }

  // aqui vamos descobrir em qual cluster tá o pointer do arquivo!!!!
  int file_clusters = 1 + ceil2(file->record.bytesFileSize/(SECTOR_SIZE* (superblock->SectorsPerCluster))); // tem no minimo 1 // era SECTOR_SIZE * 4
  int pointed_cluster = ceil2(file->pointer/(SECTOR_SIZE * (superblock->SectorsPerCluster))); // era SECTOR_SIZE * 4
  //printf("ta apontando pro cluster %d e tem %d clusters\n", pointed_cluster, file_clusters);

  if (pointed_cluster >= file_clusters) { // nunca vai entrar aqui mas ok
    //printf("da onde tanto cluster??");
    return ERROR;
  }

  // aqui vamos limpar as entradas da fat depois desse cluster se tiver mais de 1

  int current_cluster = file->record.firstCluster;

  //printf("o arquivo começa no cluster %u\n", current_cluster);
  int next_cluster;
  int visited_clusters = 0;
  int deleted_clusters = 0;


  int is_first_cluster = 1; // isso é porque o primeiro cluster a gente só trunca enquanto o resto n




  if (pointed_cluster > 0) {

    while (visited_clusters < pointed_cluster) { // coloca como current_cluster o que ta sendo apontado agora
      current_cluster = read_fat_entry(current_cluster);
      //printf("foi pro cluster %d\n", current_cluster);
      visited_clusters++;
    }
  }

  next_cluster = read_fat_entry(current_cluster); // salva o proximo cluster a visitar
  //printf("vai excluir %d clusters\n", file_clusters - pointed_cluster);

  while (deleted_clusters+visited_clusters < file_clusters - pointed_cluster) {
    //printf("while\n");

    next_cluster = read_fat_entry(current_cluster); // le na fat qual o proximo

    if (is_first_cluster) {
      //printf("vai truncar o cluster %d\n", current_cluster);
      set_fat_entry(current_cluster, -1); // diz que o cluster atual é o ultimo do arquivo
      truncate_cluster(current_cluster, file->pointer % (SECTOR_SIZE*superblock->SectorsPerCluster)); // complicado mudar isso, n sei se vale
      is_first_cluster = 0;
    }

    else {
      //printf("vai deletar o cluster %d\n", current_cluster);
      set_fat_entry(current_cluster, 0); // coloca que o cluster atual ta livre
      zero_cluster(current_cluster); // escreve 0 em todo o cluster
    }

    //printf("vai pro %d\n", next_cluster);
    current_cluster = next_cluster; // vai pro proximo cluster
    deleted_clusters++;
  }


  file->record.bytesFileSize = file->pointer;
  seek2(handle, file->record.bytesFileSize);
  //printf("o arquivo ficou com %d bytes e com pointer = %d\n", file->record.bytesFileSize, file->pointer);

  return 0;
}

int seek2 (FILE2 handle, unsigned int offset) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (handle < 0 || handle > MAX_ITEMS_IN_OPEN_LIST) {
    // printf("handle fora dos limites, man\n");
    return ERROR;
  }

  else {

    GENERIC_FILE *file;
    file = get_record_at_index(open_files, handle);

    if (file == NULL) {
      // printf("O handle %i non ecziste\n", handle);
      return ERROR;
    }

    if (file->record.bytesFileSize < offset && offset != -1) {
      // printf("o arquivo nao é tao grande, champs\n");
      return ERROR;
    }

    else {
      if (offset == -1) {
        //printf("ei\n");
        file->pointer = file->record.bytesFileSize; // poe na ultima posicao
      }
      else {
        file->pointer = offset;
      }

      return SUCESS;
    }

  }
}

int mkdir2 (char *pathname) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (strcmp(pathname, "/") == 0) {
    //printf("Erro: tentou criar o root\n");
    return ERROR;
  }

  char *name = malloc(sizeof(pathname));
  char *father_path = malloc(sizeof(pathname));

  father_path = get_father_dir_path(pathname);
  name = (char *) get_filename_from_path(pathname);


  int father_cluster_index = get_initial_cluster_from_path(father_path);

  // pega lista de entradas do diretório pai do arquivo a ser criado
  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(father_cluster_index, &files_in_father_dir);

  // checa se já existe dir com este nome
  struct t2fs_record *rec = find_record(files_in_father_dir, name);
  if (rec != NULL && rec->TypeVal == TYPEVAL_DIRETORIO) {
    //printf("\nErro ao criar dir %s.\nJá existe um arquivo com este nome neste diretório.\n\n", pathname);
    return ERROR;
  }

  // ========== criação do novo elemento da lista ==========

  // firstCluster
  int fat_entry = get_first_fat_entry_available();

  if (set_fat_entry(fat_entry, -1) == -1 || fat_entry < 0) {
    //printf("Erro ao setar fat entry como ff\n");
    return ERROR;
  }

  // cria um record
  struct t2fs_record *record;
  int bytes_per_cluster = SECTOR_SIZE * superblock->SectorsPerCluster;
  record = (struct t2fs_record *) create_record(TYPEVAL_DIRETORIO, name, bytes_per_cluster, fat_entry);

  // cria um elemento pra botar na lista
  GENERIC_FILE *new_file;
  new_file = (GENERIC_FILE *) create_generic_new_file(record, -1, 0);

  // insere o arquivo
  insert_record(&files_in_father_dir, *new_file);

  // salva no disco a nova lista de conteudo do dir pai
  write_list_of_records_to_cluster(files_in_father_dir, father_cluster_index);

  // cria as entradas . e .. no diretorio
  RECORDS_LIST *new_dir_records = newList();

  // cria entradas dos dir . e .. no novo dir
  create_default_records_in_directory(&new_dir_records, record->firstCluster, father_cluster_index);

  // e escreve o cluster desse novo dir no disco
  write_list_of_records_to_cluster(new_dir_records, record->firstCluster);


  // daqui pra baixo é só pra teste mesmo!
  RECORDS_LIST *dir = newList();

  read_all_records(father_cluster_index, &dir);
  //printf("\n\n=== %s ===\n", father_path);
  // print_records(dir);

  read_all_records(record->firstCluster, &dir);
  //printf("\n\n=== %s ===\n", pathname);
  // print_records(dir);
}

int rmdir2 (char *pathname) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }


  // considerando que vai excluir sempre do current_path (nao pode incluir o path no nome do diretorio):
  DIR2 dir = opendir2(pathname);

  if (dir == -1)
    return ERROR;


    // tem que dar um chdir2 pra dentro do diretorio pra achar os arquivos la
    chdir2(pathname);

    if (strcmp(pathname, "/") == 0 || strcmp(pathname, ".") == 0 || strcmp(pathname, "..") == 0) {
      return ERROR;
    }

    char *name = malloc(sizeof(pathname));
    name = (char *) get_filename_from_path(pathname);

    int cluster_index = get_initial_cluster_from_path(pathname);

    RECORDS_LIST *files_in_father_dir = newList();
    read_all_records(cluster_index, &files_in_father_dir);

    delete_all_records(&files_in_father_dir);
    write_list_of_records_to_cluster(files_in_father_dir, cluster_index);

    chdir2("..");
    closedir2(dir);

    // falta tirar do diretorio pai
    RECORDS_LIST *brothers = newList();
    int father_index = get_initial_cluster_from_path(current_path);
    read_all_records(father_index, &brothers);
    remove_record_at_filename(&brothers, pathname);
    write_list_of_records_to_cluster(brothers, father_index);

    //set_fat_entry(cluster_index, 0);
}

int chdir2 (char *pathname) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  char *path_copy = malloc(sizeof(char) * (1 + strlen(pathname)));
  char *buffer;
  int subresult = 0;

  strcpy(path_copy, pathname);
  buffer = strtok(path_copy, "/");

  if (subcd(buffer) == 0) {

    while ((buffer = strtok(NULL, "/")) != NULL && subresult == 0) {
      subresult = subcd(buffer);
    }
  }

  if (subresult == 0) {
    return SUCESS;
  }

  else {
    return ERROR;
  }
}

int getcwd2 (char *pathname, int size) {

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  if (size < strlen(current_path)) {
    return ERROR;
  }

  else {
    strcpy(pathname, current_path);
    return SUCESS;
  }
}


DIR2 opendir2 (char *pathname) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_dirs);

  // confere se ainda tem espaço na lista
  if (length >= MAX_ITEMS_IN_OPEN_LIST) {
    //printf("\n=======\nLista com nro máximo de elementos, man\n=======\n");
    return ERROR;
  }

  // se chegou aqui, pode colocar na lista
  char *dirname_without_path = malloc(sizeof(pathname));
  char *father_path = malloc(sizeof(pathname));

  father_path = get_father_dir_path(pathname);
  dirname_without_path = (char *) get_filename_from_path(pathname);

  int cluster_index = get_initial_cluster_from_path(father_path);

  // pega lista de entradas do diretório pai do arquivo em questão
  RECORDS_LIST *files_in_father_dir = newList();
  read_all_records(cluster_index, &files_in_father_dir);

  struct t2fs_record *record;
  record = find_record(files_in_father_dir, dirname_without_path);

  if (record == NULL) {
    //printf("Erro ao pegar record do arquivo %s\n", dirname_without_path);
    return ERROR;
  }

  int handler_available = get_fisrt_handler_available(open_dirs, MAX_ITEMS_IN_OPEN_LIST);

  // provavelmente não vai cair aqui pois se não tem espaço na lista já deve ter caído fora
  // mas é bom garantir..
  if (handler_available < 0) {
    //printf("Erro: todos handles estão ocupados\n");
    return ERROR;
  }

  // cria um elemento pra botar na lista
  GENERIC_FILE *generic_file;
  generic_file = (GENERIC_FILE *) create_generic_new_file(record, handler_available, 0);

  // insere o arquivo
  insert_record(&open_dirs, *generic_file);

  // length = list_length(open_dirs);
  //
  // //printf("\n===== open dirs =====\n");
  // print_records(open_dirs);
  // //printf("\nList length: %i\n", length);
  //
  // //printf("======================\n");

  return handler_available;
}


int readdir2 (DIR2 handle, DIRENT2 *dentry) {

  if (handle < 0 || handle >= MAX_ITEMS_IN_OPEN_LIST) {
    //printf("handle fora dos limites ou sei lá, man\n");
    return ERROR;
  }

  GENERIC_FILE *dir_generic_file = (GENERIC_FILE *) get_record_with_handle(open_dirs, handle);
  if (dir_generic_file == NULL) {
    //printf("erro ao pegar record\n");
    return -2; // OUTRO ERRO, nao é end_of_dir
  }

  // pega entradas do record que acabou de ler
  RECORDS_LIST *records_in_dir;
  read_all_records(dir_generic_file->record.firstCluster, &records_in_dir);

  // pega o record no índice indicado pelo pointer do record do dir
  GENERIC_FILE *current_file = get_record_at_index(records_in_dir, dir_generic_file->pointer);

  if (current_file == NULL) {
    //printf("não há mais records válidos\n");
    return -1; // END_OF_DIR
  }

  char    name[MAX_FILE_NAME_SIZE+1]; /* Nome do arquivo cuja entrada foi lida do disco      */
  BYTE    fileType;                   /* Tipo do arquivo: regular (0x01) ou diret�rio (0x02) */
  DWORD   fileSize;                   /* Numero de bytes do arquivo                          */

  // bota as coisa no dentry
  strcpy(dentry->name, current_file->record.name);
  dentry->fileType = current_file->record.TypeVal;
  dentry->fileSize = current_file->record.bytesFileSize;

  dir_generic_file->pointer += 1;

  return SUCESS;
}

int closedir2 (DIR2 handle) {
  /*
  1. encontra o arquivo com handle == handle na lista de arquivos abertos
  1.2 se nao encontrar -> erro
  2. encontra o cluster onde se encontra o ponteiro do record
  3. initialCluster <- entrada na fat do cluster encontrado em 2
  4. tira handle da lista de arquivos abertos
  5. free ponteiro record ??? ???
  */

  if (!has_initialized) {
    init();
    has_initialized = 1;
  }

  int length = list_length(open_dirs);

  if (handle < 0 || handle >= MAX_ITEMS_IN_OPEN_LIST) {
    //printf("handle fora dos limites ou sei lá, man\n");
    return ERROR;
  }

  if (remove_record_at_index(&open_dirs, handle) != 0) {
    // printf("Erro ao fechar dir %i\n", handle);
    return ERROR;
  }

  //printf("\n\nDID CLOSE\n\n");

  length = list_length(open_dirs);

  //printf("\n===== open dirs =====\n");
  // print_records(open_dirs);
  //printf("\nList length: %i\n", length);

  return SUCESS;
}
