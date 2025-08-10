
<font size="7"><center>**OPENSSL**</center></font>

___

[TOC]

___

## <font color="1E90FF">一、openssl 基础</font>

摘要也称哈希值，表示输入任意长度的数据，都会输出固定长度的数据。通过摘要算法（比如MDS和SHA-1）就可以得到该哈希值。

摘要算法不是加密算法，不能用于加密，因为无法通过摘要反推明文，只能用于防篡改
对摘要进行加密就是数字签名

有几种常见的哈希算法，如MD5、SHA-1、SHA-256等

<table>
  <tr><td width="0%"><font color=white>算法</td><td width="0%"><font color=white>长度</td></tr>
  <tr><td width="0%"><font color=white>MD5</td><td width="0%"><font color=white>128 bit</td></tr>
  <tr><td width="0%"><font color=white>SHA-1</td><td width="0%"><font color=white>160 bit</td></tr>
  <tr><td width="0%"><font color=white>SHA-256</td><td width="0%"><font color=white>256 bit</td></tr>
</table>





<!-- ### <font color="1E90FF">socket 套接字的类型</font> -->







## <font color="1E90FF">参考资料</font>

- [C语言中文网](http://c.biancheng.net/socket/)