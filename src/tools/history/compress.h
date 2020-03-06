#ifndef COMPRESS_H
#define COMPRESS_H

size_t hst_compressed_length(struct history *hist);

void hst_compress(struct history *hist);

#endif /* !  COMPRESS_H */