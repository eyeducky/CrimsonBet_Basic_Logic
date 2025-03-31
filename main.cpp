#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAND_SIZE 5
#define DECK_SIZE 52

#define HEALTH_POINT 4

int turn_status = 0; //0 = player, 1 = opponant


typedef struct {
    int rank; // 0~12 (2~Ace)
    int suit; // 0~3 (스페이드, 하트, 다이아, 클로버)
} Card;

void shuffle_deck(Card* deck) {
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void deal_hand(Card* deck, int* deck_index, Card* hand) {
    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i] = deck[(*deck_index)++];
    }
}

void print_hand(Card* hand) {
    char ranks[15] = "23456789TJQKA";
    char suits[5] = "SHDC"; // Spades, Hearts, Diamonds, Clubs
    for (int i = 0; i < HAND_SIZE; i++) {
        printf("%c%c ", ranks[hand[i].rank], suits[hand[i].suit]);
    }
    printf("\n");
}

void draw_cards(Card* deck, int* deck_index, Card* hand) {
    int num_draws;
    printf("(아이템 사용도 여기서 가능)몇 장을 교체하시겠습니까? (0~5): ");
    scanf("%d", &num_draws);
    for (int i = 0; i < num_draws; i++) {
        int index;
        printf("교체할 카드 번호 (1~5): ");
        scanf("%d", &index);
        hand[index - 1] = deck[(*deck_index)++];
    }
}

int evaluate_hand(Card* hand) {
    // 간단한 포커 족보 판별 (원페어, 투페어, 트리플 등)
    int ranks[13] = { 0 };
    for (int i = 0; i < HAND_SIZE; i++) {
        ranks[hand[i].rank]++;
    }
    int pairs = 0, triples = 0, fours = 0;
    for (int i = 0; i < 13; i++) {
        if (ranks[i] == 2) pairs++;
        if (ranks[i] == 3) triples++;
        if (ranks[i] == 4) fours++;
    }
    if (fours) return 7;    // 포카드
    if (triples && pairs) return 6; // 풀하우스
    if (triples) return 3;   // 트리플
    if (pairs == 2) return 2; // 투페어
    if (pairs == 1) return 1; // 원페어
    return 0; // 노페어
}

int main() {
    int player_hp = HEALTH_POINT;
    int opponent_hp = HEALTH_POINT;


    while (1) {


        srand(time(NULL));
        Card deck[DECK_SIZE];
        int deck_index = 0;

        for (int i = 0; i < DECK_SIZE; i++) {
            deck[i].rank = i % 13;
            deck[i].suit = i / 13;
        }

        turn_status = ~turn_status;

        if (turn_status) {
            printf("My turn\n");
        }
        else {
            printf("Opponent turn\n");
        }

        shuffle_deck(deck);

        Card player_hand[HAND_SIZE], dealer_hand[HAND_SIZE];
        deal_hand(deck, &deck_index, player_hand);
        deal_hand(deck, &deck_index, dealer_hand);


        printf("플레이어의 카드: ");
        print_hand(player_hand);
        draw_cards(deck, &deck_index, player_hand);

        printf("\n교체 후 플레이어의 카드: ");
        print_hand(player_hand);

        int choose;
        printf("숫자를 선택하세요\n");
        printf("1 : Bet /2 : Check /3 : Fold\n");
        scanf("%d", &choose);
        printf("(원래는 보여주지 않음)상대의 카드: ");

        print_hand(dealer_hand);

        int player_score = evaluate_hand(player_hand);
        int dealer_score = evaluate_hand(dealer_hand);

        printf("\n");
        if (turn_status) {

            //내턴일 경우
            if (player_score > dealer_score) {
                if (choose == 1) { //BET
                    printf("내가 총을 자신에게 겨눠서 쐈고, 빈 탄환이었습니다.\n");
                    printf("이에 안심하고 상대에게 총 2번을 쐈습니다\n");
                    printf("상대 HP -2\n");
                    opponent_hp -= 2;
                }
                if (choose == 2) { //CHECK
                    printf("총을 상대에게 겨눠서 쐈고, 실탄이었습니다.\n");
                    printf("상대 HP -1\n");
                    opponent_hp -= 1;
                }
                if (choose == 3) { //FOLD
                    printf("내가 총을 자신에게 겨눠서 쐈고, 실탄이었습니다.\n");
                    printf("내 HP -1\n");
                    player_hp -= 1;

                }
            }
            else if (player_score < dealer_score) {
                if (choose == 1) { //BET
                    printf("내가 총을 자신에게 겨눠서 쐈고, 실탄이었습니다.\n");
                    printf("내 HP -1\n");
                    player_hp -= 1;
                }
                if (choose == 2) { //CHECK
                    printf("총을 상대에게 겨눠서 쐈고, 빈 탄환이었습니다.\n");
                    printf("아무일도 벌어지지 않음\n");
                }
                if (choose == 3) { //FOLD
                    printf("내가 총을 자신에게 겨눠서 쐈고, 빈 탄환이었습니다.\n");
                    printf("내 턴 유지\n");
                    turn_status = ~turn_status;
                }
            }
            else {
                printf("무승부!(에러 방지용 - 모양에 따른 우열이 아직 구현되지 않아서 나옴, 원래는 없어용)_made by quack\n");
            }
        }
        else {

            //상대턴일 경우
            if (dealer_score > player_score) {
                if (choose == 1) { //BET
                    printf("상대가 총을 자신에게 겨눠서 쐈고, 빈 탄환이었습니다.\n");
                    printf("이에 안심하고 나에게 총 2번을 쐈습니다\n");
                    printf("내 HP -2\n");
                    player_hp -= 2;
                }
                if (choose == 2) { //CHECK
                    printf("총을 나에게 겨눠서 쐈고, 실탄이었습니다.\n");
                    printf("내 HP -1\n");
                    player_hp -= 1;
                }
                if (choose == 3) { //FOLD
                    printf("상대가 총을 자신에게 겨눠서 쐈고, 실탄이었습니다.\n");
                    printf("상대 HP -1\n");
                    opponent_hp -= 1;
                }
            }
            else if (dealer_score < player_score) {
                if (choose == 1) { //BET
                    printf("상대가 총을 자신에게 겨눠서 쐈고, 실탄이었습니다.\n");
                    printf("상대 HP -1\n");
                    opponent_hp -= 1;
                }
                if (choose == 2) { //CHECK
                    printf("총을 나에게 겨눠서 쐈고, 빈 탄환이었습니다.\n");
                    printf("아무일도 벌어지지 않음\n");
                }
                if (choose == 3) {
                    printf("총을 자신에게 겨눠서 쐈고, 빈 탄환이었습니다.\n");
                    printf("상대턴 유지\n");
                    turn_status = ~turn_status;
                }
            }
            else {
                printf("무승부!(에러 방지용 - 모양에 따른 우열이 아직 구현되지 않아서 나옴, 원래는 없어용)_made by quack\n");
            }
        }
        printf("\n\n");
        printf("Your Hp : %d\n", player_hp);
        printf("Opponent Hp : %d\n\n", opponent_hp);


        if (player_hp <= 0) {
            printf("You Lose");
            break;
        }
        if (opponent_hp <= 0) {
            printf("YOU WIN!");
            break;
        }

    } //while(1)
    return 0;
}
