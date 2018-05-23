#include <stdio.h>
#include "net.h"

int main() {
    int lol = 14;
    struct ListItem *item = new_item(&lol);
    printf("%d\n",*(int*) item->data);
    return 0;
}
