/*
 * Test simple du Bearer Token Twitter
 * -----------------------------------
 * Ce programme teste votre Bearer Token avec des requêtes basiques
 * pour vérifier si l'authentification fonctionne.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define BEARER_TOKEN "VOTRE BEARER TOKEN"

// Structure pour stocker les données reçues
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback pour écrire les données reçues
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Erreur: pas assez de mémoire (realloc retourne NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Fonction pour tester une requête API
void testApiRequest(const char *url, const char *description) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    struct curl_slist *headers = NULL;
    char auth_header[512];
    long response_code;

    printf("\n=== TEST: %s ===\n", description);
    printf("URL: %s\n", url);

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (!curl) {
        printf("❌ Erreur lors de l'initialisation de curl\n");
        free(chunk.memory);
        return;
    }

    // Configuration de l'en-tête d'authentification
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", BEARER_TOKEN);
    headers = curl_slist_append(headers, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Configuration de la requête
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "TwitterBotDetector/1.0");

    // Exécution de la requête
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    if (res != CURLE_OK) {
        printf(" Echec de la requete curl: %s\n", curl_easy_strerror(res));
    } else {
        printf(" Code de reponse HTTP: %ld\n", response_code);

        switch (response_code) {
            case 200:
                printf(" Succes! Réponse reçue.\n");
                printf(" Taille de la reponse: %zu bytes\n", chunk.size);

                // Afficher les premiers 500 caractères de la réponse
                if (chunk.size > 0) {
                    printf(" Debut de la reponse:\n");
                    int display_length = (chunk.size > 500) ? 500 : (int)chunk.size;
                    printf("%..*s", display_length, chunk.memory);
                    if (chunk.size > 500) {
                        printf("...\n[reponse tronquee]");
                    }
                    printf("\n");
                }
                break;

            case 401:
                printf(" Erreur d'authentification (401)\n");
                printf("   Votre Bearer Token est invalide ou expire.\n");
                printf("   Vérifiez vos clés sur developer.twitter.com\n");
                break;

            case 403:
                printf(" Acces interdit (403)\n");
                printf("   Votre application n'a pas les permissions necessaires.\n");
                printf("   Ou vous avez atteint les limites de l'API gratuite.\n");
                break;

            case 429:
                printf(" Limite de taux dépassée (429)\n");
                printf("   Trop de requêtes. Attendez avant de réessayer.\n");
                break;

            default:
                printf(" Erreur HTTP: %ld\n", response_code);
                if (chunk.size > 0) {
                    printf(" Message d'erreur:\n%s\n", chunk.memory);
                }
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(chunk.memory);
}

int main() {
    printf("=== TEST DU BEARER TOKEN TWITTER ===\n");
    printf("Token utilise: %s\n", BEARER_TOKEN);

    // Initialisation de CURL
    curl_global_init(CURL_GLOBAL_ALL);

    // Test 1: Récupération d'informations sur l'utilisateur actuel (me)
    testApiRequest("https://api.twitter.com/2/users/me",
                   "Informations sur le compte authentifié");

    // Test 2: Recherche de tweets récents (requête simple)
    testApiRequest("https://api.twitter.com/2/tweets/search/recent?query=hello&max_results=10",
                   "Recherche de tweets récents");

    // Test 3: Récupération d'un utilisateur par nom
    testApiRequest("https://api.twitter.com/2/users/by/username/twitter",
                   "Informations sur @twitter");

    // Test 4: Récupération des tweets d'un utilisateur spécifique
    testApiRequest("https://api.twitter.com/2/users/by/username/twitter",
                   "ID de l'utilisateur Twitter");

    printf("\n=== RÉSUMÉ ET RECOMMANDATIONS ===\n");
    printf("Si tous les tests echouent avec 401:\n");
    printf("   Votre Bearer Token est invalide\n");
    printf("   Regenerez vos cles sur developer.twitter.com\n\n");

    printf("Si vous obtenez 403 sur certaines requetes:\n");
    printf("   L'API gratuite a des limitations importantes\n");
    printf("   Les commentaires/reponses ne sont pas accessibles gratuitement\n");
    printf("   Considerez l'API v1.1 ou un abonnement payant\n\n");

    printf("Si vous obtenez 200 sur les tests basiques:\n");
    printf("   Votre token fonctionne pour les requetes de base\n");
    printf("   Mais l'acces aux commentaires reste limite\n");
    printf("   Utilisez la version avec donnees de test pour le developpement\n\n");

    // Nettoyage
    curl_global_cleanup();

    return 0;
}
