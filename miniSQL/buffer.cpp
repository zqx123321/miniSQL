#include "buffer.h"

Buffer* BufferManager;

Buffer::Buffer(string c, string r, string i)
	: catalogPath(c), recordPath(r), indexPath(i) {


}

Page::Page() {

}

Page::~Page() {

}

File::File(unsigned int i, string p) : ID(i), path(p) {}


File::~File() {
	delete head;
}