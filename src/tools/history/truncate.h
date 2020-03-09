#ifndef TRUNCATE_H
#define TRUNCATE_H

void hst_truncate(struct history *hist, size_t index);

void hst_truncate_uncompressed(struct history *hist, size_t count);

#endif /* !  TRUNCATE_H */