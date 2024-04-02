#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace readers {

class Motivator {
public:
    Motivator() {
        readers_current_page_.push_back(0); // сразу добавляем 0 индекс, так как id читателя начинается с 1
        readers_with_less_pages_.push_back(0);
    }

    void Read(const int reader_id, const int page) {
        int prev_page = 0;
        if (readers_current_page_.size() <= reader_id) {
            readers_current_page_.resize(reader_id + 1);
            readers_current_page_[reader_id] = page;
        } else {
            prev_page = readers_current_page_[reader_id];
            readers_current_page_[reader_id] = page;
        }

        RecalculateReadersWithLessPages(prev_page, page);
    }

    int GetUserCurrPage(int reader_id) const {
        if (readers_current_page_.size() > reader_id) {
            return readers_current_page_.at(reader_id);
        }

        return 0;
    }

    double CountLessReadingCoef(int reader_id) const {
        if (GetUserCurrPage(reader_id) == 0) {
            return 0;
        } else if (readers_with_less_pages_.at(GetUserCurrPage(reader_id)) == 1) {
            return 1;
        }

        double readers_same_page = readers_with_less_pages_.at(GetUserCurrPage(reader_id));
        double readers_amount = readers_with_less_pages_.at(1);
        double read_coef = (readers_same_page - 1) / (readers_amount - 1);
        return 1 - read_coef;
    }

private:
    std::vector<int> readers_current_page_;
    std::vector<int> readers_with_less_pages_;

    void RecalculateReadersWithLessPages(const int prev_page, const int cur_page) {
        for (int i = prev_page + 1;
             i <= std::min(cur_page, static_cast<int>(readers_with_less_pages_.size() - 1));
             ++i) {
            ++readers_with_less_pages_[i];
        }

        if (readers_with_less_pages_.size() < cur_page + 1) {
            readers_with_less_pages_.resize(cur_page + 1, 1);
        }

    }
};

} //namespase readers

void HandleReadRequest(readers::Motivator& motivator) {
    int reader_id, page;
    std::cin >> reader_id >> page;
    motivator.Read(reader_id, page);
}

void HandleCheerRequest(const readers::Motivator& motivator) {
    int reader_id;
    std::cin >> reader_id;
    std::cout << std::setprecision(6) << motivator.CountLessReadingCoef(reader_id) << std::endl;
}

int main() {
    readers::Motivator book_motivator;

    int requests_amount;
    std::cin >> requests_amount;
    
    for (; requests_amount > 0; --requests_amount) {
        using namespace std::literals;

        std::string request;
        std::cin >> request;
        if (request == "READ"s) {
            HandleReadRequest(book_motivator);
        } else if (request == "CHEER"s) {
            HandleCheerRequest(book_motivator);
        }
    }
    return 0;
}
