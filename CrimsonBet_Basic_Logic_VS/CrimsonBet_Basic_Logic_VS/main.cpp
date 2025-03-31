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
    int suit; // 0~3 (�����̵�, ��Ʈ, ���̾�, Ŭ�ι�)
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
    printf("(������ ��뵵 ���⼭ ����)�� ���� ��ü�Ͻðڽ��ϱ�? (0~5): ");
    scanf("%d", &num_draws);
    for (int i = 0; i < num_draws; i++) {
        int index;
        printf("��ü�� ī�� ��ȣ (1~5): ");
        scanf("%d", &index);
        hand[index - 1] = deck[(*deck_index)++];
    }
}

int evaluate_hand(Card* hand) {
    // ������ ��Ŀ ���� �Ǻ� (�����, �����, Ʈ���� ��)
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
    if (fours) return 7;    // ��ī��
    if (triples && pairs) return 6; // Ǯ�Ͽ콺
    if (triples) return 3;   // Ʈ����
    if (pairs == 2) return 2; // �����
    if (pairs == 1) return 1; // �����
    return 0; // �����
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


        printf("�÷��̾��� ī��: ");
        print_hand(player_hand);
        draw_cards(deck, &deck_index, player_hand);

        printf("\n��ü �� �÷��̾��� ī��: ");
        print_hand(player_hand);

        int choose;
        printf("���ڸ� �����ϼ���\n");
        printf("1 : Bet /2 : Check /3 : Fold\n");
        scanf("%d", &choose);
        printf("(������ �������� ����)����� ī��: ");

        print_hand(dealer_hand);

        int player_score = evaluate_hand(player_hand);
        int dealer_score = evaluate_hand(dealer_hand);

        printf("\n");
        if (turn_status) {

            //������ ���
            if (player_score > dealer_score) {
                if (choose == 1) { //BET
                    printf("���� ���� �ڽſ��� �ܴ��� ����, �� źȯ�̾����ϴ�.\n");
                    printf("�̿� �Ƚ��ϰ� ��뿡�� �� 2���� �����ϴ�\n");
                    printf("��� HP -2\n");
                    opponent_hp -= 2;
                }
                if (choose == 2) { //CHECK
                    printf("���� ��뿡�� �ܴ��� ����, ��ź�̾����ϴ�.\n");
                    printf("��� HP -1\n");
                    opponent_hp -= 1;
                }
                if (choose == 3) { //FOLD
                    printf("���� ���� �ڽſ��� �ܴ��� ����, ��ź�̾����ϴ�.\n");
                    printf("�� HP -1\n");
                    player_hp -= 1;

                }
            }
            else if (player_score < dealer_score) {
                if (choose == 1) { //BET
                    printf("���� ���� �ڽſ��� �ܴ��� ����, ��ź�̾����ϴ�.\n");
                    printf("�� HP -1\n");
                    player_hp -= 1;
                }
                if (choose == 2) { //CHECK
                    printf("���� ��뿡�� �ܴ��� ����, �� źȯ�̾����ϴ�.\n");
                    printf("�ƹ��ϵ� �������� ����\n");
                }
                if (choose == 3) { //FOLD
                    printf("���� ���� �ڽſ��� �ܴ��� ����, �� źȯ�̾����ϴ�.\n");
                    printf("�� �� ����\n");
                    turn_status = ~turn_status;
                }
            }
            else {
                printf("���º�!(���� ������ - ��翡 ���� �쿭�� ���� �������� �ʾƼ� ����, ������ �����)_made by quack\n");
            }
        }
        else {

            //������� ���
            if (dealer_score > player_score) {
                if (choose == 1) { //BET
                    printf("��밡 ���� �ڽſ��� �ܴ��� ����, �� źȯ�̾����ϴ�.\n");
                    printf("�̿� �Ƚ��ϰ� ������ �� 2���� �����ϴ�\n");
                    printf("�� HP -2\n");
                    player_hp -= 2;
                }
                if (choose == 2) { //CHECK
                    printf("���� ������ �ܴ��� ����, ��ź�̾����ϴ�.\n");
                    printf("�� HP -1\n");
                    player_hp -= 1;
                }
                if (choose == 3) { //FOLD
                    printf("��밡 ���� �ڽſ��� �ܴ��� ����, ��ź�̾����ϴ�.\n");
                    printf("��� HP -1\n");
                    opponent_hp -= 1;
                }
            }
            else if (dealer_score < player_score) {
                if (choose == 1) { //BET
                    printf("��밡 ���� �ڽſ��� �ܴ��� ����, ��ź�̾����ϴ�.\n");
                    printf("��� HP -1\n");
                    opponent_hp -= 1;
                }
                if (choose == 2) { //CHECK
                    printf("���� ������ �ܴ��� ����, �� źȯ�̾����ϴ�.\n");
                    printf("�ƹ��ϵ� �������� ����\n");
                }
                if (choose == 3) {
                    printf("���� �ڽſ��� �ܴ��� ����, �� źȯ�̾����ϴ�.\n");
                    printf("����� ����\n");
                    turn_status = ~turn_status;
                }
            }
            else {
                printf("���º�!(���� ������ - ��翡 ���� �쿭�� ���� �������� �ʾƼ� ����, ������ �����)_made by quack\n");
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