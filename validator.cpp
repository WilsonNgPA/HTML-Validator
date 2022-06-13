#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "stack.h"

// Tag struct to keep track of position and tag name
struct tag {
    std::string tagName;
    int position;
};

// Create and return a new tag with the data members specified
tag newTag(std::string tagName, int position) {
    tag newTag = {tagName, position};
    return newTag;
}

// Converts a string to all lower case
void convertLower(std::string &tag) {
    for(unsigned i = 0; i < tag.size(); i++) {
        tag[i] = std::tolower(tag[i]);
    }
}

// Returns true if parameter passed in is an empty tag. False otherwise
bool isEmptyTag(std::string tag) {
    convertLower(tag);
    if(tag == "<br/>") {
        return true;
    }
    return false;
}

// Returns true if parameter passed in is a closing tag. False otherwise
bool isCloseTag(std::string tag) {
    convertLower(tag);
    if(tag == "</html>" || tag == "</head>" || tag == "</body>" || tag == "</p>" || tag == "</br>" || tag == "</li>" || tag == "</h1>" || tag == "</h2>" || tag == "</ul>" || tag == "</ol>") {
        return true;
    }
    return false;
}

// Returns true if parameter passed in is a opening tag. False otherwise
bool isOpenTag(std::string tag) {
    convertLower(tag);
    if(tag == "<html>" || tag == "<head>" || tag == "<body>" || tag == "<p>" || tag == "<br>" || tag == "<li>" || tag == "<h1>" || tag == "<h2>" || tag == "<ul>" || tag == "<ol>") {
        return true;
    }
    return false;
}

// Returns a string with all forward slash removed from the original parameter string
std::string removeForwardSlash(std::string str) {
    std::string result = "";
    for(unsigned i = 0; i < str.size(); i++) {
        if(str[i] != '/') {
            result += str[i];
        }
    }
    return result;
}

// Returns a string with all spaces removed from the original parameter string
std::string removeSpacesFromString(std::string str) {
    std::string result = "";
    for(unsigned i = 0; i < str.length(); i++) {
        if(str[i] != ' ') {
            result += str[i];
        }
    }
    return result;
}

// Removes all new line character from a str. Referenced from stack overflow: https://stackoverflow.com/questions/1488775/c-remove-new-line-from-multiline-string
void removeNewLine(std::string &str) {
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
}

// Read and store the entire file into a string and returns it
std::string readHTMLFile(std::string filename) {
    std::ifstream in;
    std::ostringstream oss;
    in.open(filename);
    if(in.is_open()) {
        oss << in.rdbuf();
    }
    in.close();
    return oss.str();
}

// Get all tags from string, push them into a vector and return the vector
std::vector<tag> getTags(std::string allTags) {
    std::vector<tag> tagVector;
    // Pointer to current location in str
    int initial = 0;
    // Look for first '<' starting from current location in string
    int starting = allTags.find("<", initial);
    // While not end of str
    while (starting != static_cast<int>(std::string::npos)) {
        // Find first '>' from starting position
        int ending = allTags.find(">", starting + 1);
        // Push the substring between '<' and '>' into a vector
        tagVector.push_back(newTag(allTags.substr(starting, ending - starting + 1), starting));
        // Move the initial starting position to current ending position
        initial = ending + 1;   
        // Find first '>' from current location again
        starting = allTags.find("<", initial);
    }
    // Return the vector of tags
    return tagVector;
}

// Returns true if all tags are valid and match. False otherwise
bool validateTags(std::vector<tag> allTags) {
    stack<tag> tagStack;
    // Traverse the vector from left to right
    for(unsigned i = 0; i < allTags.size(); i++) {
        // If tag at i is an open tag, push into stack
        if(isOpenTag(allTags[i].tagName)) {
            tagStack.push(allTags[i]);
        }
        // If tag at i is a close tag
        else if(isCloseTag(allTags[i].tagName)) {
            // Remove forward slash from close tag
            std::string temp = removeForwardSlash(allTags[i].tagName);
            // If temp and top of stack matches, pop the stack
            if(temp == tagStack.peek().tagName) {
                tagStack.pop();
            }
            // If temp and top of stack does not match, print error and return false
            else {
                std::cout << "Error located at position: " << allTags[i].position << std::endl;
                std::cout << "Cause of error: " << allTags[i].tagName << std::endl;
                return false;
            }
        }
        // If tag at i is a empty tag, do nothing
        else if(isEmptyTag(allTags[i].tagName)) {

        }
        // If tag is unrecognised, print error and return false
        else {
            std::cout << "Error located at position: " << allTags[i].position << std::endl;
            std::cout << "Cause of error: " << allTags[i].tagName << std::endl;
            return false;
        }
    }

    // If stack is empty, all tags match, return true
    if(tagStack.isEmpty()) {
        return true;
    }
    // If stack is not empty, pop and print error until empty
    else {
        while(!tagStack.isEmpty()) {
            std::cout << "Error located at position: " << tagStack.peek().position << std::endl;
            std::cout << "No matching tag for: " << tagStack.peek().tagName << std::endl;
            tagStack.pop();
        }
        return false;
    }
}


int main() {
    // Reading in from file
    std::string filename;
    std::cout << "Enter filename" << std::endl;;
    std::cin >> filename;
    std::string allTags = readHTMLFile(filename);

    // Removing all spaces and print
    allTags = removeSpacesFromString(allTags);
    std::cout << std::endl;
    std::cout << "HTML file: " << std::endl;
    std::cout << allTags << std::endl;
    std::cout << std::endl;

    // Remove all new line character
    removeNewLine(allTags);

    // Push all tags in the string to a vector
    std::vector<tag> tagVector = getTags(allTags);

    // Validate the tags, print valid if true, invalid if false
    if(validateTags(tagVector)) {
        std::cout << "HTML file is valid" << std::endl;
    }
    else {
        std::cout << "HTML file is invalid" << std::endl;
    }
    return 0;
}
