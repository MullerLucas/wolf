#include "../utils.h"
#include "word_trie.h"
#include <functional>

namespace wolf {

// ----------------------------------------------

WordTrie::WordTrie()
{
    root_ = new WordTrieNode();
}

WordTrie::~WordTrie()
{
    clear();
}

WTSession WordTrie::create_session() const
{
    return WTSession {
        .valid_prefix = "",
        .valid_node = root_
    };
}

void WordTrie::insert_all(const std::string *first, const std::string *last)
{
    for (; first != last; ++first)
        (void)insert(*first);
}

// NOTE(lm): assumes word is not already in trie (no duplicates)
void WordTrie::insert(const std::string &word)
{
    WordTrieNode *curr = root_;

    for (char ch : word) {
        if (curr->children.find(ch) == curr->children.end()) {
            curr->children[ch] = new WordTrieNode();
            curr->children[ch]->parent = curr;
        }
        curr->word_count += 1;
        curr = curr->children[ch];
    }

    curr->word_count += 1;
    curr->word = &word;
}

void WordTrie::clear()
{
    traverse_postorder(root_, [](WordTrieNode* node) {
        delete node;
    });
}

void WordTrie::push(WTSession *session, const std::string &prefix) const
{
    const auto session_is_valid = session->is_valid();
    session->depth += prefix.size();

    if (!session_is_valid) return;

    const auto tmp = find(session->valid_node, prefix);
    // NOTE(lm): if the search failed, record the new depth but don't change
    //           the valid node
    if (tmp == nullptr) return;

    session->valid_prefix += prefix;
    session->valid_node = tmp;
}

void WordTrie::pop(WTSession *session, usize count) const
{
    if (session->depth == 0) return;

    count = std::min(count, session->depth);
    session->depth -= count;

    const usize size = session->valid_prefix.size();
    for (usize i = session->depth; i < size; i++) {
        session->valid_prefix.pop_back();
        session->valid_node = session->valid_node->parent;
    }
}

void WordTrie::collect(WTSession *session,
                       std::vector<const std::string*> &collector,
                       usize offset) const
{
    if (!session->is_valid()) return;
    collect_rec(session, session->valid_node, collector, offset);
}

const WordTrieNode *WordTrie::find(const WordTrieNode *node,
                                   const std::string &prefix) const
{
    const WordTrieNode *curr = node;

    for (char ch : prefix) {
        if (curr->children.find(ch) == curr->children.end())
            return nullptr;
        curr = curr->children.at(ch);
    }

    return curr;
}

void WordTrie::collect_rec(WTSession *session, const WordTrieNode *node,
                           std::vector<const std::string*> &collector,
                           usize &offset) const
{
    if (node->word) {
        collector[offset] = node->word;
        offset += 1;
    }

    for (const auto& [key, child] : node->children)
        collect_rec(session, child, collector, offset);
}

void WordTrie::traverse_postorder(WordTrieNode *node,
                                  std::function<void(WordTrieNode*)> cb)
{
    for (const auto& [key, child] : node->children)
        traverse_postorder(child, cb);
    cb(node);
}

// ----------------------------------------------

}

