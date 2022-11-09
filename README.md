## TODO List:
- Initialize sockets with SSL encryption (since GOST doesn't provide asymmetric algorithms, I'll be using RSA 2048 with OpenSSL)

-- Apparently, I have to fix ```EVP_PKEY_get_raw_private_key```, since it doesn't provide interface for 2048-bit RSA. 



- Create proper window for Client and separate it from server window 
- Make XML and INI file support 
- Make Image support
- Make support for OpenSSL public key reload
