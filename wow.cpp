#include <iostream>
#include <map>
#include <string>
#include<cmath>
#include<iomanip>
#include<queue>
using namespace std;

//节点结构
struct Node {
    char character;
    double probability;
    Node* left;
    Node* right;

    Node(char c, double p) 
        : character(c), probability(p), left(nullptr), right(nullptr) 
    {}
};

//比较函数，重载运算符。用于优先队列
struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->probability > b->probability;
    }
};


//构建赫夫曼树
Node* buildHuffmanTree(const map<char, double>& charProbabilities) {
    // 创建优先队列
    priority_queue<Node*, vector<Node*>, CompareNode> queue;

    // 将所有字符节点放入优先队列
    for (auto it = charProbabilities.begin(); it != charProbabilities.end(); ++it) {
        char character = it->first;
        double probability = it->second;
        queue.push(new Node(character, probability));
    }

    // 构建赫夫曼树
    while (queue.size() > 1) {
        // 从队列中取出概率最小的两个节点
        Node* left = queue.top();
        queue.pop();
        Node* right = queue.top();
        queue.pop();

        // 创建新的父节点
        Node* parent = new Node('$', left->probability + right->probability);
        parent->left = left;
        parent->right = right;

        // 将父节点放回队列
        queue.push(parent);
    }

    // 返回根节点
    return queue.top();
}

// 生成赫夫曼编码
map<char, string> generateHuffmanCodes(Node* root, map<char, string>& codes, string code = "") {
    if (root->left == nullptr && root->right == nullptr) {
        codes[root->character] = code;
        return codes;
    }
    if (root->left != nullptr) {
        generateHuffmanCodes(root->left, codes, code + "0");
    }
    if (root->right != nullptr) {
        generateHuffmanCodes(root->right, codes, code + "1");
    }
    return codes;
}

void EntropyCount1(std::map<char,double> M)
{
    double Entropy = 0;
    for (auto v : M)
    {
        Entropy += v.second * std::log2(v.second);
    }
    std::cout << "所选文章的所有字母和空格的平均熵H1为:" << -Entropy << std::endl;
}

void EntropyCount2(std::map<char, double> M)
{
    map<string, double> combinedProbabilities;
    double Entropy = 0.0;
    //按照标准格式打印每个字符出现的概率
    for (auto const& it : M) {
        char character = it.first;
        double probability = it.second;
        std::cout << "p(" << character << ")=" << fixed << setprecision(7) << probability << std::endl;
    }

    //计算两个字符组合的概率
    for (auto const& it1 : M)
    {
        char char1 = it1.first;
        double prob1 = it1.second;
        for (auto const& it2 : M)
        {
            char char2 = it2.first;
            double prob2 = it2.second;
            //std::cout << "p(" << char1 << char2 << ")=" << fixed << setprecision(7) << prob1 * prob2 << std::endl;
            string combination = std::string(1, char1) + std::string(1, char2);
            combinedProbabilities[combination] = prob1 * prob2;
        }
    }
    for (auto const& it : combinedProbabilities) {
        std::cout << "p(" << it.first << ")=" << fixed << std::setprecision(7) << it.second << std::endl;
    }

    for (auto v : combinedProbabilities) {
        Entropy += v.second * std::log2(v.second);
    }

    std::cout << "所选文章的联合信息熵H2为:" << -Entropy << std::endl;
    //std::cout << "totalprobability is " << totalProbabilitity << std::endl;
}

//编码字符串
string encodeString(const string& str, const map<char, string>& huffmanCodes) 
{
    string encodedString = "";
    for (char c : str) 
    {
        encodedString += huffmanCodes.at(c);
    }
    return encodedString;
}

//字符串译码
char decode(Node* root, const string& code, int& index) {
    if (root->left == nullptr && root->right == nullptr) {
        return root->character;
    }
    if (code[index] == '0') {
        index++;
        return decode(root->left, code, index);
    }
    else {
        index++;
        return decode(root->right, code, index);
    }
}


//处理字符串
std::string convertToLower(std::string str) {
    std::string convertedString;
    for (char c : str) {
        if (std::isalpha(c)) {
            convertedString += tolower(c);
        }
        else if (isspace(c)) {
            convertedString += ' ';
        }
    }
    return convertedString;
}

/*
A bank robber stole a lot of money. He was caught and sent to prison, but the money was never found. When he came out of prison, they watched him to see what he would do.Here is the detective, reporting to the inspector. 'Yes, sir, I found Johnny . I followed him all around the town, but frankly, I couldn't make anything out of what he bought. Here's the list.'
shirt , heavy crowbar, box of chocolates , shovel, heavy hammer,bunch of flowers .The inspector said, 'Good. That helps me a lot. Do you remember how we watched him helping his neighbour, old Mrs. Judson to cover her backyard with cement?

*/

/*
a  a  b  b
*/
int main() {
    // 声明字符串变量存储用户输入的文本
    string text;
    cout << "请输入文本：";
    getline(cin, text);

    //去除标点，仅保留空格，将大写字母变为小写
    std::string target=convertToLower(text);

    // 创建映射存储字符及其出现的次数
    map<char, int> occurrences;
    for (char character : target) {
    //for (char character : text) {
        // 判断字符是否为字母或空格
        if (isalpha(character)) {
            // 将字符转换为小写并更新相应字符的出现次数
            occurrences[tolower(character)]++;
        }
        else if (character == ' ') {
            // 更新空格的出现次数
            occurrences[' ']++;
        }
    }

    // 计算总字符数
    int totalCharacters = 0;
    for (auto pair : occurrences) {
        totalCharacters += pair.second;
    }

    // 创建映射存储字符及其出现的概率
    map<char, double> probabilities;
    for (auto pair : occurrences) {
        // 计算每个字符的概率
        double probability = (double)pair.second / totalCharacters;
        // 将概率存储在映射中
        probabilities[pair.first] = probability;
    }

    //构建赫夫曼树
    Node* root = buildHuffmanTree(probabilities);

    std::map<char, string> huffmanCodes;
    generateHuffmanCodes(root, huffmanCodes);

    for (auto it = huffmanCodes.begin(); it != huffmanCodes.end(); ++it) {
        char character = it->first;
        string code = it->second;
        cout << character << ": " << code << endl;
    }

    //计算平均码长
    double averageCodeLength = 0;
    for (auto it = huffmanCodes.begin(); it != huffmanCodes.end(); ++it) {
        char character = it->first;
        string code = it->second;
        averageCodeLength += probabilities[character] * code.length();
    }
    std::cout << "平均码长：" << averageCodeLength << std::endl;

    //计算编码效率
    double codingEfficiency = totalCharacters / (averageCodeLength * totalCharacters);
    std::cout << "编码效率：" << codingEfficiency << std::endl;

    
    // 编码字符串
    string encodedString = encodeString(target, huffmanCodes);

    // 打印编码结果
    cout << "原始字符串: " << target << endl;
    cout << "编码后的字符串: " << encodedString << endl;


    //字符串译码
    string decodedString = "";
    int index = 0;
    while (index < encodedString.length()) {
        decodedString += decode(root, encodedString, index);
    }
    std::cout << "译码后的字符串：" << decodedString << std::endl;


    // 打印字符概率
    //cout << "字符概率：" << endl;
    //for (auto pair : probabilities) {
    //    cout << pair.first << ": " << pair.second << endl;
    //}

    // 打印总字符数和空格数
    //cout << "总字符数和空格数：" << totalCharacters << endl;

    //EntropyCount1(probabilities);
    
    //EntropyCount2(probabilities);
    return 0;
}
