#include <ares/text.h>

namespace ares {
//    bool BuilderText::stoppable(char value) {
//        return std::isspace(value) || (std::find(hard.begin(), hard.end(), value) != hard.end());
//    }

    size_t BuilderText::get() {
        return index;
    }

    void BuilderText::set(size_t value) {
        index = value;
    }

    void BuilderText::push() {
        while (index < text.size() && std::isspace(text[index])) {
            index++;
        }
    }

    std::string BuilderText::pull(size_t size) {
        return text.substr(index, std::min(size, text.size() - index));
    }

    void BuilderText::pop(size_t size) {
        index += size;

        push();
    }

    size_t BuilderText::until(const Stoppable &stoppable) {
        size_t size = 0;

        if (stoppable(&text[index], text.size() - index)) {
            return 1;
        }

        while ((index + size) < text.size()) {
            if (stoppable(&text[index + size], text.size() - index - size)) {
                break;
            }

            size++;
        }

        return size;
    }

    bool BuilderText::complete(size_t size, const Stoppable &stoppable) {
        return (index + size >= text.size()) || stoppable(&text[index + size], text.size() - index - size);
    }

    BuilderText::BuilderText(std::string text) : text(std::move(text)) {
        push();
    }
}
