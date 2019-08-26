# masterlist_parser

```
usage : ./masterlist_parser -in file [options]
where options are
 -inform fmt          input format (DER or PEM (default))
 -find file(s)        find if certs (DER) are in masterlist
 --export-all         export all certificates in separate files
 --export-signer      export masterlist signer certificate
 --export-CSCA        export CSCA which signed the masterlist
 --print-infos        print infos of all certificates
 --print-validity     print validity of masterlist signer
 ```
