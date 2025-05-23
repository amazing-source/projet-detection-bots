#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Version de test avec commentaires prédéfinis (simulant l'API Twitter)
const char* test_comments[] = {
    "Hey! I make $5000 per day trading crypto! DM me for details!",
    "Great article, thanks for sharing your insights.",
    "AMAZING INVESTMENT OPPORTUNITY!!! Contact me NOW for INSTANT PROFITS!!!",
    "I love this post, very informative and well written.",
    "Changed my life! Thanks to @trader123 I'm rich! Message me for info!",
    "Really interesting perspective on this topic.",
    "FREE MONEY!!! Click here to earn $1000 daily! GUARANTEED!!!",
    "Thanks for posting this, learned something new today.",
    "Bitcoin trading made me wealthy! WhatsApp me +1234567890",
    "Nice photo! Love the colors and composition.",
    "URGENT: First 10 people get FREE investment advice! Hurry!",
    "Could you share more details about this research?",
    "Earn from home! Real opportunity! Contact me with message 'MONEY'",
    "Congratulations on this achievement, well deserved!",
    "My testimony: I was struggling financially until I found this system!"
};

// Fonction pour compter les occurrences d'une sous-chaîne
int count_substring(const char* text, const char* pattern) {
    int count = 0;
    const char* pos = text;

    while ((pos = strstr(pos, pattern)) != NULL) {
        count++;
        pos++;
    }
    return count;
}

// Fonction pour vérifier si le texte contient un pattern (insensible à la casse)
int contains_pattern(const char* text, const char* pattern) {
    char* text_lower = malloc(strlen(text) + 1);
    char* pattern_lower = malloc(strlen(pattern) + 1);
    int found = 0;

    // Conversion en minuscules
    strcpy(text_lower, text);
    strcpy(pattern_lower, pattern);

    for (int i = 0; text_lower[i]; i++) {
        text_lower[i] = tolower(text_lower[i]);
    }
    for (int i = 0; pattern_lower[i]; i++) {
        pattern_lower[i] = tolower(pattern_lower[i]);
    }

    if (strstr(text_lower, pattern_lower) != NULL) {
        found = 1;
    }

    free(text_lower);
    free(pattern_lower);
    return found;
}

// Fonction principale d'analyse des bots
int analyze_bot_probability(const char* text, char* reasons) {
    int bot_score = 0;
    reasons[0] = '\0'; // Vider les raisons

    // 1. Vérification des appels à l'action suspects
    const char* action_patterns[] = {
        "dm me", "message me", "contact me", "whatsapp me",
        "text me", "call me", "reach out"
    };

    for (int i = 0; i < 7; i++) {
        if (contains_pattern(text, action_patterns[i])) {
            bot_score += 30;
            strcat(reasons, "Appel à l'action suspect; ");
            break;
        }
    }

    // 2. Vérification des termes financiers/argent
    const char* money_patterns[] = {
        "$", "money", "profit", "earn", "income", "rich",
        "wealthy", "investment", "trading", "bitcoin", "crypto"
    };

    int money_keywords = 0;
    for (int i = 0; i < 11; i++) {
        if (contains_pattern(text, money_patterns[i])) {
            money_keywords++;
        }
    }

    if (money_keywords >= 1) {
        bot_score += 25;
        strcat(reasons, "Termes financiers; ");
    }

    // 3. Vérification des exclamations excessives
    int exclamation_count = count_substring(text, "!");
    if (exclamation_count >= 3) {
        bot_score += 20;
        strcat(reasons, "Exclamations excessives; ");
    }

    // 4. Vérification du texte en majuscules
    int total_letters = 0;
    int uppercase_letters = 0;

    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            total_letters++;
            if (isupper(text[i])) {
                uppercase_letters++;
            }
        }
    }

    if (total_letters > 10 && uppercase_letters * 100 / total_letters > 50) {
        bot_score += 20;
        strcat(reasons, "TEXTE EN MAJUSCULES; ");
    }

    // 5. Vérification des mots de spam typiques
    const char* spam_patterns[] = {
        "free", "guaranteed", "instant", "urgent", "limited time",
        "amazing", "incredible", "opportunity", "click here"
    };

    for (int i = 0; i < 9; i++) {
        if (contains_pattern(text, spam_patterns[i])) {
            bot_score += 15;
            strcat(reasons, "Terme de spam; ");
            break;
        }
    }

    // 6. Vérification des patterns de témoignage d'arnaque
    const char* scam_patterns[] = {
        "changed my life", "testimony", "struggling financially",
        "made me wealthy", "thanks to", "real opportunity"
    };

    for (int i = 0; i < 6; i++) {
        if (contains_pattern(text, scam_patterns[i])) {
            bot_score += 25;
            strcat(reasons, "Narration d'arnaque; ");
            break;
        }
    }

    // 7. Vérification des URLs ou liens
    if (strstr(text, "http") || strstr(text, "www.") || strstr(text, ".com")) {
        bot_score += 10;
        strcat(reasons, "Contient des liens; ");
    }

    return bot_score;
}

int main() {
    printf("=== DETECTEUR DE BOTS TWITTER (VERSION TEST) ===\n");
    printf("Analyse de %d commentaires similaires à ceux trouvés sur Twitter\n\n", 15);

    int total_comments = 15;
    int bot_count = 0;
    int human_count = 0;

    for (int i = 0; i < total_comments; i++) {
        char reasons[500];
        int bot_score = analyze_bot_probability(test_comments[i], reasons);

        int is_bot = (bot_score >= 40); // Seuil de 40%

        if (is_bot) {
            bot_count++;
        } else {
            human_count++;
        }

        printf("--- Commentaire #%d ---\n", i + 1);
        printf("Texte: \"%s\"\n", test_comments[i]);
        printf("Score bot: %d%% - Classification: %s\n",
               bot_score, is_bot ? "BOT" : "HUMAIN");
        if (strlen(reasons) > 0) {
            printf("Raisons: %s\n", reasons);
        }
        printf("\n");
    }

    printf("=== RESULTATS FINAUX ===\n");
    printf("Total commentaires analysés: %d\n", total_comments);
    printf("Bots détectés: %d (%.1f%%)\n", bot_count, (float)bot_count * 100 / total_comments);
    printf("Humains détectés: %d (%.1f%%)\n", human_count, (float)human_count * 100 / total_comments);

    return 0;
}
