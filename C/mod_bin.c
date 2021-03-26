#include <endian.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_err() {
    _exit(2);
}

void overflow() {
    _exit(3);
}

struct Data {
    int16_t x;
    int64_t y;
}__attribute__((packed));

void marshall(unsigned char *out, const struct Data *in) {
    int16_t sx = htole16(in->x);
    int64_t sy = htole64(in->y);
    memcpy(out, &sx, sizeof(int16_t));
    memcpy(out + sizeof(int16_t), &sy, sizeof(int64_t));
}

void unmarshall(struct Data *out, const unsigned char *in) {
    memcpy(out, in, sizeof(int16_t));
    memcpy(&(out->y), in + sizeof(int16_t), sizeof(int64_t));
    out->x = le16toh(out->x);
    out->y = le64toh(out->y);
}

void perform_arithm(struct Data *op, int32_t mult) {
    int64_t ll_res;
    if (__builtin_saddl_overflow(op->y, op->x * mult, &ll_res)) {
        overflow();
    }
    op->y = ll_res;
}

void reverse_fstruct(int fd, int32_t mult) {
    ssize_t file_sz = (lseek(fd, 0, SEEK_END) -
            lseek(fd, 0, SEEK_SET)) / sizeof(struct Data);
    for (int processed_structs = 0; processed_structs < file_sz / 2 + file_sz % 2; ++processed_structs) {
        struct Data buf;
        unsigned char fs[sizeof(struct Data)], ss[sizeof(struct Data)];
        // stored structs
        if (lseek(fd, processed_structs * sizeof(struct Data), SEEK_SET) == -1) {
            file_err();
        }

        if (read(fd, fs, sizeof(struct Data)) != sizeof(struct Data)) {
            file_err();
        }

        if (lseek(fd, -(processed_structs + 1) * sizeof(struct Data), SEEK_END) == -1) {
            file_err();
        }

        if (read(fd, ss, sizeof(struct Data)) != sizeof(struct Data)) {
            file_err();
        }

        // processed first struct
        unmarshall(&buf, fs);
        perform_arithm(&buf, mult);
        marshall(fs, &buf);

        // processed second struct
        unmarshall(&buf, ss);
        perform_arithm(&buf, mult);
        marshall(ss, &buf);

        // swapped structs
        if (lseek(fd, processed_structs * sizeof(struct Data), SEEK_SET) == -1) {
            file_err();
        }

        if (write(fd, ss, sizeof(struct Data)) != sizeof(struct Data)) {
            file_err();
        }

        if (lseek(fd, -(processed_structs + 1) * sizeof(struct Data), SEEK_END) == -1) {
            file_err();
        }

        if (write(fd, fs, sizeof(struct Data)) != sizeof(struct Data)) {
            file_err();
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "incorrect number of cmd line args\n");
        _exit(1);
    }

    char *c;
    int32_t mult = strtol(argv[2], &c, 10);

    int fd = open(argv[1], O_RDWR);
    reverse_fstruct(fd, mult);
    close(fd);
}