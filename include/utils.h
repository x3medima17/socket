#pragma once

namespace Utils {

    template<class T>
    std::shared_ptr<HttpMessage> get_http_request(Socket &client)
    {
            std::string data("");
            std::pair<int,std::string> tmp;
            do
            {
                    tmp = client.recv(128);
            data += tmp.second;
            if(tmp.first < 2)
                break;
        } while(data.substr(data.size()-4,4) != "\r\n\r\n" && tmp.first == 128 );

        // Check for content
        int clen = 0;
        if(data.find("Content-Length: ") != std::string::npos)
        {
                int p = data.find("Content-Length:");
                int l = data.find(" ",p);
                int r = data.find("\r\n",p);
                clen = std::stoi( data.substr(l+1, r-l) );
        }
        if(clen > 0)
           data += client.recv(clen).second;
        return std::shared_ptr<HttpMessage>(new T(data));
    }

    const std::vector<std::string> HttpHeaders= {"GET", "POST"};
}

