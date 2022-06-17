# 加密解密

Java API：

- Cipher.getInstance(transformation)，“算法/模式/填充”或“算法”，如 AES/CBC/PKCS5Padding；
  - PKCS7Padding：PKCS #7 填充字符串由一个字节序列组成，每个字节填充该填充字节序列的长度，0x01-0x08，根据最后一位知道填充字节数；
  - PKCS5Padding的blocksize为8字节，PKCS7Padding的blocksize可以为1到255字节；