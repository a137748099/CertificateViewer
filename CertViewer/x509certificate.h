#ifndef X509CERTIFICATE_H
#define X509CERTIFICATE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(X509CERTIFICATE_LIBRARY)
#  define X509CERTIFICATE_EXPORT Q_DECL_EXPORT
#else
#  define X509CERTIFICATE_EXPORT Q_DECL_IMPORT
#endif

using namespace std;

class X509Certificate_p;
class X509CertificateExtension;
class X509CertificateExtension_p;
class X509CERTIFICATE_EXPORT X509Certificate
{
public:
    X509Certificate();
    X509Certificate(const char *cert, int len);
    X509Certificate(const X509Certificate &other);

    enum HashType {
        Hash_Sha1,
        Hash_Sha256,
        Hash_SM3
    };

    enum SubjectInfo {
        Organization,
        CommonName,
        LocalityName,
        OrganizationalUnitName,
        CountryName,
        StateOrProvinceName,
        DistinguishedNameQualifier,
        SerialNumber,
        EmailAddress
    };

    X509Certificate &operator=(const X509Certificate &other);

    bool operator==(const X509Certificate &other) const;
    inline bool operator!=(const X509Certificate &other) const { return !operator==(other); }

    bool isNull() const;
    void *handle() const;

    int version() const;
    string serialNumber() const;

    string subject() const;
    string subjectInfo(SubjectInfo subject) const;
    string subjectDisplyName() const;

    string issuer() const;
    string issuerInfo(SubjectInfo issuer) const;
    string issuerDisplyName() const;

    string notBefor() const;
    string notAfter() const;

    string digest(HashType type) const;

    string publicKeyValue() const;
    string publicKeyType() const;

    string signAlgType() const;
    string signValue() const;

    vector<X509CertificateExtension> extensions() const;

    string toDer() const;
    string toPem() const;
    string toText() const;

    static bool importPkcs12(const char *pfxFile, int len, char *priKey, int &priKeyLen,
                            X509Certificate *x509Cert, vector<X509Certificate> &caCerts,
                            const char *pass = "");
    static bool importP7b(const char *p7b, int len, vector<X509Certificate> &caCerts);

    static vector<X509Certificate *> splitCertChain(const string &chains);//only pem

    static int verify(const X509Certificate &userCert, vector<X509Certificate> certificateChain);

    static vector<X509Certificate> systemCaCertificates();
private:
    shared_ptr<X509Certificate_p> p;
    friend class X509Certificate_p;
};



class X509CERTIFICATE_EXPORT X509CertificateExtension
{
public:
    X509CertificateExtension();

    enum ExtMethod {
        Ext_String,
        Ext_Vector,
        Ext_Map
    };

    bool isCritical() const;
    bool isSupported() const;
    string name() const;
    string oid() const;
    string value() const;

    ExtMethod methodType() const;
    string toString() const;
    vector<string> toVector() const;
    multimap<string, string> toMap() const;

private:
    X509CertificateExtension_p *p = nullptr;

    friend class X509Certificate;
};

#endif // X509CERTIFICATE_H
