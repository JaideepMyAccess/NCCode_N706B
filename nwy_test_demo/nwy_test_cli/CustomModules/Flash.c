// #include "jsmn.h"
#include "global.h"
#include "nwy_adapt_platform.h"

// #define NWY_RDONLY 0x0
// #define	NWY_RDONLY	  0x0
// #define	NWY_WRONLY	  0x1
// #define	NWY_RDWR	 0x2
// #define	NWY_CREAT	  0x40
// #define	NWY_TRUNC	 0x04
// #define	NWY_APPEND	  0x400

#define MAX_TOKENS 20


// extern int sta, stb, ham, hbo, bct, hur, min, bcc;
// extern int iid, itp;
// extern char qrb_data[1024] ;
// extern const char qrb_array[512];
// extern char MAC_ID[10];

typedef enum {
  JSMN_UNDEFINED = 0, 
  JSMN_OBJECT = 1 << 0,
  JSMN_ARRAY = 1 << 1,
  JSMN_STRING = 1 << 2,
  JSMN_PRIMITIVE = 1 << 3
} jsmntype_t;
// JSMN
typedef struct jsmntok {
  jsmntype_t type;
  int start;
  int end;
  int size;
#ifdef JSMN_PARENT_LINKS
  int parent;
#endif
} jsmntok_t;

typedef struct {
    jsmntok_t tokens[MAX_TOKENS];
    char *json;
    int token_count;
} JsonParser;

enum jsmnerr {
  /* Not enough tokens were provided */
  JSMN_ERROR_NOMEM = -1,
  /* Invalid character inside JSON string */
  JSMN_ERROR_INVAL = -2,
  /* The string is not a full JSON packet, more bytes expected */
  JSMN_ERROR_PART = -3
};

typedef struct jsmn_parser {
  unsigned int pos;     /* offset in the JSON string */
  unsigned int toknext; /* next token to allocate */
  int toksuper;         /* superior token node, e.g. parent object or array */
} jsmn_parser;

static jsmntok_t *jsmn_alloc_token(jsmn_parser *parser, jsmntok_t *tokens,
                                   const size_t num_tokens) {
  jsmntok_t *tok;
  if (parser->toknext >= num_tokens) {
    return NULL;
  }
  tok = &tokens[parser->toknext++];
  tok->start = tok->end = -1;
  tok->size = 0;
#ifdef JSMN_PARENT_LINKS
  tok->parent = -1;
#endif
  return tok;
}


// New
typedef struct {
    jsmntok_t *tokens;
    char *json;
    int token_count;
    int token_capacity;
} DynamicJsonParser;


DynamicJsonParser parse_json_dynamic(const char *json_string) {
    DynamicJsonParser parser;
    jsmn_parser jsmn;
    jsmn_init(&jsmn);

    parser.token_capacity = 128; // You can safely increase this
    parser.tokens = malloc(sizeof(jsmntok_t) * parser.token_capacity);
    parser.json = strdup(json_string);

    if (!parser.tokens || !parser.json) {
        nwy_test_cli_echo("Memory allocation failed!\n");
        if (parser.tokens) free(parser.tokens);
        if (parser.json) free(parser.json);
        parser.tokens = NULL;
        parser.json = NULL;
        parser.token_count = -1;
        return parser;
    }

    parser.token_count = jsmn_parse(&jsmn, parser.json, strlen(parser.json), parser.tokens, parser.token_capacity);
    if (parser.token_count < 0) {
        nwy_test_cli_echo("JSON parsing Fail! Code: %d\n", parser.token_count);
        free(parser.tokens);
        free(parser.json);
        parser.tokens = NULL;
        parser.json = NULL;
    }

    return parser;
}

// Function to parse JSON
JsonParser parse_json(const char *json_string) {
    JsonParser parser;
    jsmn_parser jsmn;
    jsmn_init(&jsmn);

    // Allocate memory for JSON copy
    parser.json = strdup(json_string);
    parser.token_count = jsmn_parse(&jsmn, parser.json, strlen(parser.json), parser.tokens, MAX_TOKENS);

    // nwy_test_cli_echo("Data, JSON: %s, Tokens: %d\n", parser.json, parser.tokens);

    if (parser.token_count < 0) {
        nwy_test_cli_echo("JSON parsing Fail! %d\n",parser.token_count);
        // free(parser.json);
        parser.json = NULL;
    }
    return parser;
}

// Function to get a value for a given key
char *get_json_value(JsonParser *parser, const char *key) {
    if (!parser || !parser->json) return NULL;

    for (int i = 0; i < parser->token_count; i++) {
        if (parser->tokens[i].type == JSMN_STRING &&
            strncmp(parser->json + parser->tokens[i].start, key, parser->tokens[i].end - parser->tokens[i].start) == 0) {
            
            // Extract value of the key (next token)
            int len = parser->tokens[i + 1].end - parser->tokens[i + 1].start;
            char *value = (char *)malloc(len + 1);
            strncpy(value, parser->json + parser->tokens[i + 1].start, len);
            value[len] = '\0';  // Null-terminate
            return value;
        }
    }
    return NULL;  // Key not found
}

int find_token_index(JsonParser *parser, const char *key, int start_index) {
    for (int i = start_index; i < parser->token_count; i++) {
        if (parser->tokens[i].type == JSMN_STRING &&
            strncmp(parser->json + parser->tokens[i].start, key, parser->tokens[i].end - parser->tokens[i].start) == 0) {
            return i + 1; // Return the index of the value token
        }
    }
    return -1; // Not found
}

int find_token_index_recursive(DynamicJsonParser *parser, const char *key) {
    for (int i = 0; i < parser->token_count; ++i) {
        if (parser->tokens[i].type == JSMN_STRING) {
            int length = parser->tokens[i].end - parser->tokens[i].start;
            if (strncmp(parser->json + parser->tokens[i].start, key, length) == 0 && strlen(key) == length) {
                return i + 1;  // return index of the corresponding value
            }
        }
    }
    return -1;
}

// Function to free allocated memory
void free_json_parser(JsonParser *parser) {
    if (parser->json) free(parser->json);
}


static const unsigned char base64_table[256] = {
    [0 ... 255] = 0x80, // invalid
    ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,  ['E'] = 4,
    ['F'] = 5,  ['G'] = 6,  ['H'] = 7,  ['I'] = 8,  ['J'] = 9,
    ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14,
    ['P'] = 15, ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23, ['Y'] = 24,
    ['Z'] = 25,
    ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29, ['e'] = 30,
    ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35,
    ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39, ['o'] = 40,
    ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45,
    ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49, ['y'] = 50,
    ['z'] = 51,
    ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55, ['4'] = 56,
    ['5'] = 57, ['6'] = 58, ['7'] = 59, ['8'] = 60, ['9'] = 61,
    ['+'] = 62, ['/'] = 63,
    ['='] = 0
};

int base64_decode(const char *in, unsigned char *out, int outlen) {
    int i = 0, j = 0;
    unsigned char a, b, c, d;

    while (in[i] && in[i+1] && in[i+2] && in[i+3] && j < outlen) {
        a = base64_table[(unsigned char)in[i++]];
        b = base64_table[(unsigned char)in[i++]];
        c = base64_table[(unsigned char)in[i++]];
        d = base64_table[(unsigned char)in[i++]];

        if ((a & 0x80) || (b & 0x80) || (c & 0x80) || (d & 0x80))
            return -1; // Invalid base64

        out[j++] = (a << 2) | (b >> 4);
        if (in[i-2] != '=' && j < outlen)
            out[j++] = (b << 4) | (c >> 2);
        if (in[i-1] != '=' && j < outlen)
            out[j++] = (c << 6) | d;
    }

    return j; // Number of bytes decoded
}

void print_hex(unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        nwy_test_cli_echo("0x%02X", data[i]);
        if (i < len - 1) printf(", ");
    }
    nwy_test_cli_echo("\n");
}

bool load_techconfig_values() {
    char buffer[256] = {0};
    int fd = -1, len = 0;

    if (!nwy_file_exist("techconfig")) {
        nwy_test_cli_echo("techconfig file not found!\n");
        return false;
    }

    fd = nwy_file_open("techconfig", NWY_RDONLY);
    if (fd < 0) {
        nwy_test_cli_echo("Failed to open techconfig file\n");
        return false;
    }

    len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);

    if (len <= 0) {
        nwy_test_cli_echo("techconfig file is empty or read failed\n");
        return false;
    }

    buffer[len] = '\0'; // Null-terminate to safely parse as string
    nwy_test_cli_echo("Techconfig content (%d bytes):\n%s\n", len, buffer);

    JsonParser parser = parse_json(buffer);
    if (!parser.json) {
        nwy_test_cli_echo("Failed to parse JSON in techconfig\n");
        return false;
    }

    sta = atoi(get_json_value(&parser, "sta"));
    stb = atoi(get_json_value(&parser, "stb"));
    ham = atoi(get_json_value(&parser, "ham"));
    hbo = atoi(get_json_value(&parser, "hbo"));
    bct = atoi(get_json_value(&parser, "bct"));
    hur = atoi(get_json_value(&parser, "hur"));
    min = atoi(get_json_value(&parser, "min"));
    bcc = atoi(get_json_value(&parser, "bcc"));

    nwy_test_cli_echo("TechConfig loaded: sta=%d, stb=%d, ham=%d, hbo=%d, bct=%d, hur=%d, min=%d, bcc=%d\n",
                      sta, stb, ham, hbo, bct, hur, min, bcc);

    // free_json_parser(&parser);
    return true;
}

void load_business_config_values_temp() {
strncpy(qrb_data, "//D39PT09/D//PL48/T5+/b2+PXx/fT/9fH++fL9+vT3/f76+fr6+//79/X19fn18P//8Pf09PT38P////////8X3VJXU9IV/gm6FDkbttY9RLVPLwDyHHcIugN0UnQDZw+v3S6Uo4z0znJCLsjqjWra/RTUUlJW2xr/////////xxyzlTeFVSITkXfNfy8NLhuTfnCxvmZBuO237Ls7CwjMuNVlV9nNqK7EMmCIbNyymufE+M6EzJC//////////0xfX6JgOVVFkIhXfvwRK0soW0VWuKzDoJElVNGtJYCartxEqhMjb7uqQioUj8XMGoAh3Q2IBQhUqv////////96bJwKEN5V3huMG9Xpm0Vhy7phXZq1ZnqbnAnIV88RbKhjXfdXXV1hzpGIN7zfXJoRz07EDwaEOT3/////////6uT/bwH8VUn66Nioiwdf+XJeWeUwN8KIAc2ef3UIDY7KTE0uUTTNv6vFM1euZESshYAidP8moIS6/////////wwtTe0t3VTHgvbyJnTiffBXVa9TSD2tZ7nV0Hf1R9DR7s/9gwAF7ZOuMyIBj9xN+xDXpadglohasv////////8H9xcXF/cH/xeH33+n96efJ2eP95//n58XZx9PJ3cvF6+v3xcX19cH7z8PP+93X6dfb1cXN+ePd/////////8=", sizeof(qrb_data) - 1);
  nwy_test_cli_echo("qrb_data:\n%s\n", qrb_data);

    int decoded_len = base64_decode(qrb_data, qrb_array, sizeof(qrb_array));
    if (decoded_len > 0) {
        nwy_test_cli_echo("\n\nDecoded (%d bytes):\n", decoded_len);
        print_hex(qrb_array, 512);
    } else {
        nwy_test_cli_echo("Base64 decode failed.\n");
    }

}
bool load_business_config_values() {
    char buffer[2048] = {0};  // Large buffer for QRB base64
    int fd = -1, len = 0;

    if (!nwy_file_exist("businessconfig")) {
        nwy_test_cli_echo("businessconfig file not found!\n");
        return false;
    }

    fd = nwy_file_open("businessconfig", NWY_RDONLY);
    if (fd < 0) {
        nwy_test_cli_echo("Failed to open businessconfig file\n");
        return false;
    }

    len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);

    if (len <= 0) {
        nwy_test_cli_echo("businessconfig file is empty or read failed\n");
        return false;
    }

    buffer[len] = '\0';  // Ensure null termination
    nwy_test_cli_echo("BusinessConfig content (%d bytes):\n%s\n", len, buffer);

    JsonParser parser = parse_json(buffer);
    if (!parser.json) {
        nwy_test_cli_echo("Failed to parse JSON in businessconfig\n");
        return false;
    }

    // Extract values
    char *iid_str = get_json_value(&parser, "iid");
    char *itp_str = get_json_value(&parser, "itp");
    char *qrb_str = get_json_value(&parser, "qrb");

    if (iid_str) iid = atoi(iid_str);
    if (itp_str) itp = atoi(itp_str);
    if (qrb_str) strncpy(qrb_data, qrb_str, sizeof(qrb_data) - 1);

    nwy_test_cli_echo("BusinessConfig loaded: iid=%d, itp=%d\n", iid, itp);
    nwy_test_cli_echo("qrb_data:\n%s\n", qrb_data);

    int decoded_len = base64_decode(qrb_data, qrb_array, sizeof(qrb_array));
    if (decoded_len > 0) {
        nwy_test_cli_echo("\n\nDecoded (%d bytes):\n", decoded_len);
        print_hex(qrb_array, 512);
    } else {
        nwy_test_cli_echo("Base64 decode failed.\n");
    }

    // // Cleanup
    // free_json_parser(&parser);
    // if (iid_str) free(iid_str);
    // if (itp_str) free(itp_str);
    // if (qrb_str) free(qrb_str);

    return true;
}

bool load_machine_config_values() {
    char buffer[256] = {0};
    int fd = -1, len = 0;

    if (!nwy_file_exist("machineconfig")) {
        nwy_test_cli_echo("machineconfig file not found!\n");
        return false;
    }

    fd = nwy_file_open("machineconfig", NWY_RDONLY);
    if (fd < 0) {
        nwy_test_cli_echo("Failed to open machineconfig file\n");
        return false;
    }

    len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);

    if (len <= 0) {
        nwy_test_cli_echo("machineconfig file is empty or read failed\n");
        return false;
    }

    buffer[len] = '\0';  // Null-terminate the JSON string
    nwy_test_cli_echo("MachineConfig content (%d bytes):\n%s\n", len, buffer);

    JsonParser parser = parse_json(buffer);
    if (!parser.json) {
        nwy_test_cli_echo("Failed to parse JSON in machineconfig\n");
        return false;
    }

    char *mid_str = get_json_value(&parser, "mid");
    char *mnt_str = get_json_value(&parser, "mnt");
    
    if (mid_str)
        strncpy(MAC_ID, mid_str, sizeof(MAC_ID) - 1);
    
    if (mnt_str)
        strncpy(MERCHANT_ID, mnt_str, sizeof(MERCHANT_ID) - 1);
    
    nwy_test_cli_echo("MachineConfig loaded: MAC_ID=%s, MERCHANT_ID=%s\n", MAC_ID, MERCHANT_ID);    

    // free_json_parser(&parser);
    // if (mid_str) free(mid_str);
    // if (mnt_str) free(mnt_str);

    return true;
}

void save_incin_config_values() {
    char incin_json[256];
    snprintf(incin_json, sizeof(incin_json),
             "{\"paused\":%d,\"batch_id\":\"%s\",\"cycle\":%d,\"total_burn\":%d,\"start_time\":\"%s\",\"trigger_state\":%d}",
             isIncinerationPaused,
             IncinBatchID,
             Incin_cycle,
             IncinTotalNapkinBurn,
             IncinStartTime,
             IncinTriggerState);

    int fd = nwy_file_open("incinConfig", NWY_CREAT | NWY_RDWR | NWY_TRUNC);
    if (fd >= 0) {
        int written = nwy_file_write(fd, incin_json, strlen(incin_json));
        nwy_file_close(fd);
        if (written != strlen(incin_json)) {
            nwy_test_cli_echo("Partial write to incinConfig (%d of %d bytes).\n", written, strlen(incin_json));
            return ;
        }
        nwy_test_cli_echo("IncinConfig saved: %s\n", incin_json);
        return ;
    } else {
        nwy_test_cli_echo("Failed to open incinConfig for writing.\n");
        return ;
    }
}

void load_incin_config_values() {
    char buffer[256] = {0};
    int fd = -1, len = 0;

    if (!nwy_file_exist("incinConfig")) {
        nwy_test_cli_echo("incinConfig file not found!\n");
        return ;
    }

    fd = nwy_file_open("incinConfig", NWY_RDONLY);
    if (fd < 0) {
        nwy_test_cli_echo("Failed to open incinConfig file\n");
        return ;
    }

    len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);

    if (len <= 0) {
        nwy_test_cli_echo("incinConfig file is empty or read failed\n");
        return ;
    }

    buffer[len] = '\0'; // Ensure null-termination
    nwy_test_cli_echo("IncinConfig content (%d bytes):\n%s\n", len, buffer);

    JsonParser parser = parse_json(buffer);
    if (!parser.json) {
        nwy_test_cli_echo("Failed to parse JSON in incinConfig\n");
        return ;
    }

    char *paused_str = get_json_value(&parser, "paused");
    char *batch_str = get_json_value(&parser, "batch_id");
    char *cycle_str = get_json_value(&parser, "cycle");
    char *burn_str = get_json_value(&parser, "total_burn");
    char *start_str = get_json_value(&parser, "start_time");
    char *trigger_str = get_json_value(&parser, "trigger_state");

    if (paused_str) isIncinerationPaused = atoi(paused_str);
    if (batch_str) strncpy(IncinBatchID, batch_str, sizeof(IncinBatchID) - 1);
    if (cycle_str) Incin_cycle = atoi(cycle_str);
    if (burn_str) IncinTotalNapkinBurn = atoi(burn_str);
    if (start_str) strncpy(IncinStartTime, start_str, sizeof(IncinStartTime) - 1);
    if (trigger_str) IncinTriggerState = atoi(trigger_str);

    nwy_test_cli_echo("IncinConfig loaded: paused=%d, batch_id=%s, cycle=%d, total_burn=%d, start_time=%s, trigger_state=%d\n",
        isIncinerationPaused, IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, IncinTriggerState);

    // free_json_parser(&parser);
    // if (paused_str) free(paused_str);
    // if (batch_str) free(batch_str);
    // if (cycle_str) free(cycle_str);
    // if (burn_str) free(burn_str);
    // if (start_str) free(start_str);
    // if (trigger_str) free(trigger_str);

    return ;
}

void delete_file_by_name(const char *file_name) {
    if (!nwy_file_exist(file_name)) {
        nwy_test_cli_echo("\r\nFile '%s' does not exist.\r\n", file_name);
        return;
    }

    if (nwy_file_remove(file_name) == 0) {
        nwy_test_cli_echo("\r\nFile '%s' deleted successfully.\r\n", file_name);
    } else {
        nwy_test_cli_echo("\r\nFailed to delete file '%s'.\r\n", file_name);
    }
}
