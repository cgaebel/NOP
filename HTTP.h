#pragma once

// It is the caller's responsibility to escape any necessary strings to avoid weird bugs.
std::string HTTPEscape(const std::string& toEscape);

// Request the url... returns the server's response. If response == "failed", the function dealt with the error.
std::string HTTPGet(std::string url);
std::string HTTPPost(std::string url);
