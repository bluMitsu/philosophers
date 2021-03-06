/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bharghaz <bharghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 22:05:59 by bharghaz          #+#    #+#             */
/*   Updated: 2022/01/25 22:06:01 by bharghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	choose_number(const char *str, int sign)
{
	unsigned long long int	num;
	int						i;

	num = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			num = num * 10 + (str[i] - 48);
			i++;
		}
		else
			return (0);
	}
	if (sign == -1 && num > 9223372036854775807)
		return (0);
	if (num > 9223372036854775807)
		return (-1);
	return (num);
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	i;

	i = 0;
	sign = 1;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '-')
		return (0);
	else if (str[i] == '+')
		i++;
	if (str[i] >= '0' && str[i] <= '9')
		return (sign * choose_number(&str[i], sign));
	else
		return (0);
}
